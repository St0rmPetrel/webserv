#include <string>

#include "ServerMux.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "IHandler.hpp"

using namespace http;

void ServerMux::serve_http(Response& res, Request& req) const {
	(void)res;
	(void)req;
}

void ServerMux::handle(const std::string& pattern, const IHandler& handler) {
	(void)pattern;
	(void)handler;
}
