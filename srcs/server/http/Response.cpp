#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const {
	return "HTTP/1.1 200 OK\r\n"
		"Content-Length: 12\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"Hello world!";
}

void Response::write_header(Response::StatusCode code) {
	(void)code;
}

void Response::write(const char* begin, const char* end) {
	(void)begin;
	(void)end;
}

void Response::Header::set(const std::string& key, const std::string& value) {
	(void)key;
	(void)value;
}

Response::StatusCode http::int_to_status_code(int status_code) {
	switch (status_code) {
		case 400:
			return Response::BadRequest;
		case 404:
			return Response::NotFound;
		case 405:
			return Response::MethodNotAllowed;
		default:
			// TODO make exception
			throw "";
	}
}
