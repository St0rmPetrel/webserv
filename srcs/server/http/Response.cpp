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
