#include "FileServerHandler.hpp"

using namespace http;

FileServerHandler::FileServerHandler(const logger::Logger& log, const Options& opts)
	: _log(log), _opts(opts) { }

FileServerHandler::FileServerHandler(const FileServerHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

FileServerHandler::~FileServerHandler() { }

void FileServerHandler::serve_http(Response& res, const Request& req) const {
	if (!path_is_valid(req.path)) {
		this->bad_request(res, req);
		return ;
	}

	if (req.method == http::method_get) {
		this->get_file(res, req);
	} else if (req.method == http::method_post) {
		this->post_file(res, req);
	} else if (req.method == http::method_delete) {
		this->delete_file(res, req);
	} else {
		this->method_not_allowed(res, req);
	}
	return ;
}


IHandler* FileServerHandler::clone() const {
	return (new FileServerHandler(*this));
}

void FileServerHandler::bad_request(Response& res, const Request& req) const {
	(void)req;
	res.write_header(Response::BadRequest);
}

void FileServerHandler::not_found(Response& res, const Request& req) const {
	(void)req;
	res.write_header(Response::NotFound);
}

void FileServerHandler::method_not_allowed(Response& res, const Request& req) const {
	(void)req;
	res.write_header(Response::MethodNotAllowed);
}

void FileServerHandler::internal_server_error(Response& res, const Request& req) const {
	(void)req;
	res.write_header(Response::InternalServerError);
}

bool FileServerHandler::path_is_valid(const std::string& path) {
	(void)path;
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
