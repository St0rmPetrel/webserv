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

void CGIHandler::serve_http(Response& res, const Request& req) const {
	std::string script_path = _opts.root + req.path;
	if (!_file_exist(script_path)) {
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
	if (!_file_exist(interpretator_path)) {
		internal_server_error(res);
		return;
	}
	std::map<std::string, std::string> envp = _set_envp(req);
	std::string raw_cgi_output = _exec_cgi(interpretator_path, script_path, envp);
	_parse_cgi_output(res, raw_cgi_output);
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
	(void)req;
	std::map<std::string, std::string> envp;

	// https://www.oreilly.com/openbook/cgi/ch02_02.html
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["SERVER_NAME"] = "webserv";
	envp["SERVER_SOFTWARE"] = "";
	envp["SERVER_PROTOCOL"] = "";
	envp["SERVER_PORT"] = "";
	envp["REQUEST_METHOD"] = "";
	envp["PATH_INFO"] = "";
	envp["PATH_TRANSLATED"] = "";
	envp["SCRIPT_NAME"] = "";
	envp["DOCUMENT_ROOT"] = "";
	envp["QUERY_STRING"] = "";
	envp["REMOTE_HOST"] = "";
	envp["REMOTE_ADDR"] = "";
	envp["AUTH_TYPE"] = "";
	envp["REMOTE_USER"] = "";
	envp["REMOTE_IDENT"] = "";
	envp["CONTENT_TYPE"] = "";
	envp["CONTENT_LENGTH"] = "";
	envp["HTTP_FROM"] = "";
	envp["HTTP_ACCEPT"] = "";
	envp["HTTP_USER_AGENT"] = "";
	envp["HTTP_REFERER"] = "";

	for (std::map<std::string, std::string>::const_iterator it = _opts.params.begin();
			it != _opts.params.end(); ++it) {
		envp[it->first] = it->second;
	}

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

bool CGIHandler::_file_exist(const std::string& path) const {
	(void)path;
	return true;
}

void CGIHandler::_parse_cgi_output(Response& res, const std::string& raw_cgi_output) const {
	// TODO
	//
	if (raw_cgi_output.empty()) {
		internal_server_error(res);
		return;
	}
	res.write_header(http::Response::OK);
	res.write(raw_cgi_output, http::mime_type_txt);
}

CGIHandler::Options::Options() { }

CGIHandler::Options::Options(const Options& ref)
	: root(ref.root)
	  , extention_to_interpretator_path(ref.extention_to_interpretator_path)
	  , params(ref.params)
{ }

CGIHandler::Options::~Options() { }
