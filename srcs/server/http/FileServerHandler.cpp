#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

#include "FileServerHandler.hpp"
#include "../../utils/utils.hpp"

using namespace http;

FileServerHandler::FileServerHandler(const logger::Logger& log, const Options& opts)
	: _log(log), _opts(opts) { }

FileServerHandler::FileServerHandler(const FileServerHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

FileServerHandler::~FileServerHandler() { }

void FileServerHandler::serve_http(Response& res, const Request& req) const {
	if (!path_is_valid(req.path)) {
		this->bad_request(res);
		return ;
	}

	if (req.method == http::method_get) {
		this->get_file(res, req);
	} else if (req.method == http::method_post) {
		this->post_file(res, req);
	} else if (req.method == http::method_delete) {
		this->delete_file(res, req);
	} else {
		this->method_not_allowed(res);
	}
	return ;
}


IHandler* FileServerHandler::clone() const {
	return (new FileServerHandler(*this));
}

void FileServerHandler::bad_request(Response& res) const {
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

void FileServerHandler::not_found(Response& res) const {
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

void FileServerHandler::method_not_allowed(Response& res) const {
	res.write_header(Response::MethodNotAllowed);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>405 Not Allowed</h1>\n"
		"  <p>webserv</p>\n"
		"  <p>Method not allowed.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

void FileServerHandler::internal_server_error(Response& res) const {
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

bool FileServerHandler::path_is_valid(const std::string& path) {
	(void)path;
	// In most cases URL path validation does via regex
	// I can't do that because of the subject
	// so that simple validation check that path is absolute
	// and go up of directory tree is forbidden

	if (path.empty()) {
		return false;
	}
	if (path.find("..") != std::string::npos) {
		return false;
	}
	if (path[0] != '/') {
		return false;
	}
	return true;
}

void FileServerHandler::get_file(Response& res, const Request& req) const {
	std::string   path = _opts.root + req.path;

	_get_dir_or_file(res, req, path);
}

void FileServerHandler::_get_dir_or_file(Response& res, const Request& req, const std::string& path) const {
	bool is_directory = false;
	// check if file is directory
	{
		DIR *dir = opendir(path.c_str());
		if (dir != NULL) {
			closedir(dir);
			is_directory = true;
		}
	}
	if (is_directory) {
		_get_dir(res, req, path);
	} else {
		_get_file(res, path);
	}
}

void FileServerHandler::_get_file(Response& res, const std::string& path) const {
	std::ifstream ifs;

	_log.info(SSTR("[FileServerHandler] GET FILE path =" << path));
	ifs.open(path.c_str(), std::ifstream::in);
	if (ifs.fail()) {
		not_found(res);
		return;
	}
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	res.write_header(Response::OK);
	res.write(buffer.str(), utils::detect_file_mime_type(path));
}

void FileServerHandler::_get_dir(Response& res, const Request& req, const std::string& path) const {
	_log.info(SSTR("[FileServerHandler] GET DIR path =" << path));
	if (!_opts.autoindex && _opts.index.empty()) {
		not_found(res);
		return ;
	}
	std::set<std::string> entry_names;
	// read file names in directory
	{
		struct dirent *entry = NULL;
		DIR *dir = opendir(path.c_str());
		if (dir == NULL) {
			not_found(res);
			return;
		}
		while ((entry = readdir(dir)) != NULL) {
			std::string entry_name = std::string(entry->d_name);
			entry_names.insert(entry_name);
			_log.debug(SSTR("[FileServerHandler] find in dir=" << path << " file=" << entry_name));
		}
		closedir(dir);
	}
	// check index
	for (std::set<std::string>::const_iterator it = _opts.index.begin(); it != _opts.index.end();
			++it) {
		std::set<std::string>::const_iterator index_entry_name = entry_names.find(*it);
		if (index_entry_name != entry_names.end()) {
			_get_dir_or_file(res, req, path + "/" + *index_entry_name);
			return;
		}
	}
	// make autoindex
	if (_opts.autoindex) {
		res.write_header(Response::OK);
		std::string files_list;
		std::map<std::string, std::string>::const_iterator host = req.headers.find("host");
		if (host == req.headers.end()) {
			_log.error("[FileServerHandler] not found host header in request");
			not_found(res);
			return;
		}
		for (std::set<std::string>::const_iterator it = entry_names.begin();
				it != entry_names.end(); ++it) {
			std::string href = "<p><a href=http://" +
				host->second + req.path + "/" + *it +">"+
				*it + "</a></p>\n";
			files_list += href;
		}
		std::string body = "<html>\n"
			"<body>\n"
			"  <h1>Index of "+path+"</h1>\n" +
			files_list +
			"</body>\n"
			"</html>\n";
		res.write(body, http::mime_type_html);

	} else {
		not_found(res);
	}
}

void FileServerHandler::post_file(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

void FileServerHandler::delete_file(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

FileServerHandler::Options::Options() : autoindex(false) { }

FileServerHandler::Options::Options(const Options& ref)
	: root(ref.root), autoindex(ref.autoindex), index(ref.index) { }

FileServerHandler::Options::~Options() { }
