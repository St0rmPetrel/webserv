#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <cstdio>

#include "FileServerHandler.hpp"
#include "../../utils/utils.hpp"

using namespace http;

FileServerHandler::FileServerHandler(const logger::Logger& log, const Options& opts)
	: _log(log), _opts(opts) {

	if (_opts.root.empty()) {
		throw FileServerHandler::EmptyRootException();
	}
}

FileServerHandler::FileServerHandler(const FileServerHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

FileServerHandler::~FileServerHandler() { }

void FileServerHandler::serve_http(Response& res, const Request& req) const {
	_log.info(SSTR("[FileServerHandler] [serve_http] " << req.method << " " << req.path));
	if (!_path_is_valid(req.path)) {
		this->bad_request(res);
		return ;
	}

	if (req.method == http::method_get) {
		this->_method_get(res, req);
	} else if (req.method == http::method_post) {
		this->_method_post(res, req);
	} else if (req.method == http::method_delete) {
		this->_method_delete(res, req);
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

bool FileServerHandler::_path_is_valid(const std::string& path) const {
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

void FileServerHandler::_method_get(Response& res, const Request& req) const {
	std::string   path = _opts.root + req.path;

	_get_dir_or_file(res, req, path);
}

void FileServerHandler::_get_dir_or_file(Response& res, const Request& req, const std::string& path) const {
	if (!_is_dir(path)) {
		_log.info(SSTR("[FileServerHandler] [GET FILE] path =" << path));
		_get_file(res, path);
		return ;
	}
	_log.info(SSTR("[FileServerHandler] [GET DIR] path =" << path));
	std::set<std::string> entry_names = _dir_entry(path);
	std::string index = _get_index(entry_names);
	if (index != "") {
		_log.info(SSTR("[FileServerHandler] [GET] find index in dir: " << index));
		_get_dir_or_file(res, req, path + "/" + index);
		return;
	}
	if (_opts.autoindex) {
		_generate_autoindex(res, req, entry_names);
		return;
	}
	not_found(res);
}

void FileServerHandler::_get_file(Response& res, const std::string& path) const {
	std::ifstream ifs;

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

bool FileServerHandler::_is_dir(const std::string& path) const {
	DIR *dir = opendir(path.c_str());
	if (dir != NULL) {
		closedir(dir);
		return true;
	}
	return false;
}

std::set<std::string> FileServerHandler::_dir_entry(const std::string& path) const {
	std::set<std::string> entry_names;

	struct dirent *entry = NULL;
	DIR *dir = opendir(path.c_str());
	if (dir == NULL) {
		return entry_names;
	}
	while ((entry = readdir(dir)) != NULL) {
		std::string entry_name = std::string(entry->d_name);
		if (entry_name == "." || entry_name == "..") {
			continue;
		}
		entry_names.insert(entry_name);
	}
	closedir(dir);
	return entry_names;
}

std::string FileServerHandler::_get_index(const std::set<std::string>& dir_entry) const {
	for (std::set<std::string>::const_iterator it = _opts.index.begin(); it != _opts.index.end();
			++it) {
		std::set<std::string>::const_iterator index_entry_name = dir_entry.find(*it);
		if (index_entry_name != dir_entry.end()) {
			return *index_entry_name;
		}
	}
	return "";
}

void FileServerHandler::_generate_autoindex(Response& res, const Request& req,
		const std::set<std::string>& dir_entry) const {

	res.write_header(Response::OK);
	std::string files_list;
	std::map<std::string, std::string>::const_iterator host_header = req.headers.find("host");
	if (host_header == req.headers.end()) {
		_log.error("[FileServerHandler] [Autoindex] not found host header in request");
		not_found(res);
		return;
	}
	for (std::set<std::string>::const_iterator it = dir_entry.begin();
			it != dir_entry.end(); ++it) {
		std::string href = "<p><a href=http://" +
			host_header->second + req.path + "/" + *it +">"+
			*it + "</a></p>\n";
		files_list += href;
	}
	std::string body = "<html>\n"
		"<body>\n"
		"  <h1>Index of "+req.path+"</h1>\n" +
		files_list +
		"</body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

std::pair<std::string, std::string> FileServerHandler::_file_path_split(const std::string& path) const {
	size_t path_separator_lacation = path.find_last_of("/");

	if (path_separator_lacation == std::string::npos) {
		return std::make_pair("", path);
	}
	std::string dir_path = path.substr(0, path_separator_lacation);
	std::string file_path = path.substr(path_separator_lacation+1);
	return std::make_pair(dir_path, file_path);
}

void FileServerHandler::_method_post(Response& res, const Request& req) const {
	_log.info(SSTR("[FileServerHandler] [POST] " << req.path));
	std::pair<std::string, std::string> dir_file_path_pair = _file_path_split(req.path);
	if (dir_file_path_pair.first != "" &&
			!_is_dir(_opts.root + dir_file_path_pair.first)) {
		_log.error(SSTR("[FileServerHandler] [POST] dir does not exist: " <<
					dir_file_path_pair.first));
		res.write_header(http::Response::NoContent);
		res.write("no content", http::mime_type_txt);
		return;
	}
	std::string new_file_path = _opts.root + req.path;
	if (utils::file_exist(new_file_path)) {
		_log.warn(SSTR("[FileServerHandler] [POST] file already exist: " <<
					dir_file_path_pair.second));
		res.write_header(http::Response::OK);
		res.header.set("Location", req.path);
		res.write("file exist", http::mime_type_txt);
		return;
	}
	std::ofstream new_file;

	new_file.open(new_file_path.c_str());
	if (new_file.fail()) {
		res.write_header(http::Response::InternalServerError);
		res.write("fail to post file", http::mime_type_txt);
		return;
	}
	new_file << req.body;
	res.write_header(http::Response::Created);
	res.header.set("Location", req.path);
	res.write("file created", http::mime_type_txt);
}

void FileServerHandler::_method_delete(Response& res, const Request& req) const {
	_log.info(SSTR("[FileServerHandler] [DELETE] " << req.path));
	std::string delete_file_path = _opts.root + req.path;
	if (!utils::file_exist(delete_file_path)) {
		res.write_header(http::Response::NoContent);
		res.write("no content", http::mime_type_txt);
		return;
	}
	if (remove(delete_file_path.c_str()) != 0) {
		res.write_header(http::Response::InternalServerError);
		res.write("fail to delete file", http::mime_type_txt);
		return;
	}
	res.write_header(http::Response::OK);
	res.write("file deleted", http::mime_type_txt);
}

const char* FileServerHandler::EmptyRootException::what() const throw() {
	return "opts.root in FileServerHandler must be not empty";
}

FileServerHandler::Options::Options() : autoindex(false) { }

FileServerHandler::Options::Options(const Options& ref)
	: root(ref.root), autoindex(ref.autoindex), index(ref.index) { }

FileServerHandler::Options::~Options() { }
