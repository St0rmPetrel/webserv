#include "CGIHandler.hpp"

using namespace http;

CGIHandler::CGIHandler(const logger::Logger& log, const CGIHandler::Options& opts)
	: _log(log), _opts(opts) { }

CGIHandler::CGIHandler(const CGIHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

CGIHandler::~CGIHandler() { }

void CGIHandler::serve_http(Response& res, const Request& req) const {
	(void)res;
	(void)req;
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

CGIHandler::Options::Options() { }

CGIHandler::Options::Options(const Options& ref)
	: extention_to_interpretator_path(ref.extention_to_interpretator_path)
	  , params(ref.params)
{ }

CGIHandler::Options::~Options() { }
