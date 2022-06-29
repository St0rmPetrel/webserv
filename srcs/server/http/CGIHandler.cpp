#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <set>
#include <string>

#include "CGIHandler.hpp"
#include "../../utils/utils.hpp"

using namespace http;

CGIHandler::CGIHandler(const logger::Logger& log, const CGIHandler::Options& opts)
	: _log(log), _opts(opts)
{
	if (_opts.root.empty()) {
		throw EmptyRootException();
	}
	if (_opts.extention_to_interpretator_path.empty()) {
		throw EmptyInterpretatorException();
	}
}

CGIHandler::CGIHandler(const CGIHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

CGIHandler::~CGIHandler() { }

std::pair<std::string, std::string> CGIHandler::_script_name_path_info_pair(const std::string& path) const {
	std::string cgi_dir = "/cgi-bin/";
	size_t script_name_sep_begin = path.find(cgi_dir);
	if (script_name_sep_begin == std::string::npos) {
		return std::make_pair("", "");
	}
	script_name_sep_begin += cgi_dir.size();
	size_t script_name_sep_end = path.find("/", script_name_sep_begin);
	if (script_name_sep_end == std::string::npos) {
		return std::make_pair(path, "");
	}
	return std::make_pair(path.substr(0, script_name_sep_end), path.substr(script_name_sep_end));
}

void CGIHandler::serve_http(Response& res, const Request& req) const {
	std::pair<std::string, std::string> script_pathinfo = _script_name_path_info_pair(req.path);
	std::string script_path = _opts.root + script_pathinfo.first;
	_log.debug(SSTR("[CGIHandler] try to execute script_path=" << script_path));
	if (!utils::file_exist(script_path)) {
		not_found(res);
		return;
	}
	std::map<std::string, std::string>::const_iterator ext_to_intr_it =
		_opts.extention_to_interpretator_path.find(utils::file_extension(script_path));
	if (ext_to_intr_it == _opts.extention_to_interpretator_path.end()) {
		bad_request(res);
		return;
	}
	std::string interpretator_path = ext_to_intr_it->second;
	if (!utils::file_exist(interpretator_path)) {
		internal_server_error(res);
		return;
	}
	std::map<std::string, std::string> envp = _set_envp(req);
	std::string raw_cgi_output = _exec_cgi(interpretator_path, script_path, envp);
	_parse_cgi_output(res, raw_cgi_output);
	_log.info(SSTR("[CGIHandler] execute script_path=" << script_path));
}

IHandler* CGIHandler::clone() const {
	return (new CGIHandler(*this));
}

void CGIHandler::bad_request(Response& res) const {
	res.write_header(Response::BadRequest);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>400 Bad Request</h1>\n"
		"  <p>webserv</p>\n"
		"  <p>This page isn't work.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

void CGIHandler::not_found(Response& res) const {
	res.write_header(Response::NotFound);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>404 Not Found</h1>\n"
		"  <p>webserv</p>\n"
		"  <p>The requested URL was not found on server.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

void CGIHandler::internal_server_error(Response& res) const {
	res.write_header(Response::InternalServerError);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>500 Internal Server Error</h1>\n"
		"  <p>webserv</p>\n"
		"  <p>Sorry something goes wrong.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

const char* CGIHandler::EmptyRootException::what() const throw() {
	return "root must be not empty";
}

const char* CGIHandler::EmptyInterpretatorException::what() const throw() {
	return "extention to interpretator path must be not empty";
}

std::map<std::string, std::string> CGIHandler::_set_envp(const Request& req) const {
	std::map<std::string, std::string> envp;

	std::string host_header = _get_request_header(req, "content-type");
	std::string host, port = "80";
	size_t host_sep = host_header.find(":");
	if (host_sep != std::string::npos) {
		host = host_header.substr(0, host_sep);
		port = host_header.substr(host_sep);
	}
	std::pair<std::string, std::string> script_pathinfo = _script_name_path_info_pair(req.path);
	// https://www.oreilly.com/openbook/cgi/ch02_02.html
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["SERVER_NAME"] = host;
	envp["SERVER_SOFTWARE"] = "webserv/0.1";
	envp["SERVER_PROTOCOL"] = "HTTP/" + req.version;
	envp["SERVER_PORT"] = port;
	envp["REQUEST_METHOD"] = req.method;
	envp["PATH_INFO"] = script_pathinfo.second;
	envp["SCRIPT_NAME"] = script_pathinfo.first;
	envp["DOCUMENT_ROOT"] = _opts.root;
	envp["QUERY_STRING"] = req.query;
	envp["REMOTE_HOST"] = "";
	envp["REMOTE_ADDR"] = "";
	envp["AUTH_TYPE"] = _get_request_header(req, "authorization");
	envp["REMOTE_USER"] = "";
	envp["REMOTE_IDENT"] = "";
	envp["CONTENT_TYPE"] = _get_request_header(req, "content-type");
	envp["CONTENT_LENGTH"] = _get_request_header(req, "content-length");
	envp["HTTP_FROM"] = "";
	envp["HTTP_ACCEPT"] = "";
	envp["HTTP_USER_AGENT"] = _get_request_header(req, "user-agent");
	envp["HTTP_REFERER"] = "";

	for (std::map<std::string, std::string>::const_iterator it = _opts.params.begin();
			it != _opts.params.end(); ++it) {
		envp[it->first] = it->second;
	}
	envp["PATH_TRANSLATED"] = std::string(envp["DOCUMENT_ROOT"]) + script_pathinfo.second;

	return envp;
}

std::string CGIHandler::_exec_cgi(const std::string& interpretator_path,
		const std::string& script_path,
		const std::map<std::string, std::string>& envp) const {
	int pipefds[2];
	if (pipe(pipefds) < 0) {
		return "";
	}

	std::set<std::string> envp_set;
	for (std::map<std::string, std::string>::const_iterator cit = envp.begin();
			cit != envp.end(); ++cit) {
		std::string env_var = cit->first + "=" + cit->second;
		envp_set.insert(env_var);
	}

	int cgi_stat = 0;
	/* ATTENTION C PURE CODE UNSAFE */
	/* throw is forbidden may cause to memory leak */
	{
		char** command = NULL;
		char** environment = NULL;

		command = (char **)malloc(3 * sizeof(char*));
		command[0] = (char *)interpretator_path.c_str();
		command[1] = (char *)script_path.c_str();
		command[2] = NULL;

		environment = (char **)malloc((envp_set.size() + 1) * sizeof(char*));
		{
			int i = 0;
			for (std::set<std::string>::const_iterator cit = envp_set.begin();
					cit != envp_set.end(); ++cit) {
				environment[i] = (char *)cit->c_str();
				++i;
			}
		}
		environment[envp_set.size()] = NULL;

		pid_t pid = fork();
		if (pid < 0) {
			free(command);
			free(environment);
			return "";
		} else if (pid == 0) {
			// cgi child script
			close(pipefds[0]);
			if (dup2(pipefds[1], 1) < 0) {
				exit(1);
			}
			close(0);
			close(pipefds[1]);
			close(2);
			execve(interpretator_path.c_str(), command, environment);
			free(command);
			free(environment);
			exit(2);
		}
		close(pipefds[1]);
		wait(&cgi_stat);
		free(environment);
		free(command);
	}
	/* END OF C PURE CODE UNSAFE */
	if (cgi_stat != 0) {
		return "";
	}
	return (utils::read_file_fd(pipefds[0]));
}

void CGIHandler::_parse_cgi_output(Response& res, const std::string& raw_cgi_output) const {
	if (raw_cgi_output.empty()) {
		internal_server_error(res);
		return;
	}
	size_t header_sep = raw_cgi_output.find("\n\n");
	if (header_sep == std::string::npos) {
		internal_server_error(res);
		return;
	}
	std::string raw_headers = raw_cgi_output.substr(0, header_sep);
	std::string body = raw_cgi_output.substr(header_sep+2);

	http::Response::StatusCode status       = http::Response::OK;
	std::string                content_type = http::mime_type_txt;
	{
		std::stringstream raw_headers_stream(raw_headers);
		std::string raw_header;
		while (std::getline(raw_headers_stream, raw_header, '\n')) {
			size_t key_sep = raw_header.find(": ");
			if (key_sep == std::string::npos) {
				continue;
			}
			std::string key   = raw_header.substr(0, key_sep);
			std::string value = raw_header.substr(key_sep+2);
			if (utils::str_to_lower(key) == "content-type") {
				content_type = value;
			} else if (utils::str_to_lower(key) == "status") {
				status = http::str_to_status_code(value);
			} else {
				res.header.set(key, value);
			}
    	}
	}

	res.write_header(status);
	res.write(body, content_type);
}

std::string CGIHandler::_get_request_header(const Request& req, const std::string& key) const {
	std::map<std::string, std::string>::const_iterator header = req.headers.find(key);

	if (header == req.headers.end()) {
		return "";
	}
	return header->second;
}

CGIHandler::Options::Options() { }

CGIHandler::Options::Options(const Options& ref)
	: root(ref.root)
	  , extention_to_interpretator_path(ref.extention_to_interpretator_path)
	  , params(ref.params)
{ }

CGIHandler::Options::~Options() { }
