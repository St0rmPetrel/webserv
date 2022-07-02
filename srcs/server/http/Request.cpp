#include "Request.hpp"

#include "../../utils/utils.hpp"

using namespace http;

void http::fill_request_host_close(Request& req) {
	std::map<std::string, std::string>::const_iterator host_iterator = req.headers.find("host");
	if (host_iterator != req.headers.end()) {
		req.host = host_iterator->second;
	}
	std::map<std::string, std::string>::const_iterator close_iterator = req.headers.find("connection");
	if (close_iterator != req.headers.end()) {
		if (utils::str_to_lower(close_iterator->second) == "keep-alive") {
			req.close = false;
		}
	}
}
