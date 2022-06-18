#include "FileServerHandler.hpp"

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

	if (path.emty()) {
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
	(void)res;
	(void)req;
}

void FileServerHandler::post_file(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

void FileServerHandler::delete_file(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}
