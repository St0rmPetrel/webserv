#include <string>

#include "ServerMux.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "IHandler.hpp"

using namespace http;

ServerMux::ServerMux() { }

ServerMux::ServerMux(const ServerMux& mux) {
	(void)mux;
}

ServerMux::~ServerMux() { }

void ServerMux::serve_http(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

IHandler* ServerMux::clone() const {
	return NULL;
}

void ServerMux::bad_request(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

void ServerMux::not_found(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

ServerMux::Route ServerMux::new_route() {
	return ServerMux::Route(*this);
}

ServerMux::Route::Route(ServerMux& mux) : _mux(mux) { }

ServerMux::Route::Route(const ServerMux::Route& r) : _mux(r._mux) {
	(void)r;
}

ServerMux::Route::~Route() { }

ServerMux::Route& ServerMux::Route::method(const std::string& method) {
	(void)method;
	return (*this);
}

ServerMux::Route& ServerMux::Route::host(const std::string& host) {
	(void)host;
	return (*this);
}

ServerMux::Route& ServerMux::Route::header(const std::pair<std::string, std::string>& pair) {
	(void)pair;
	return (*this);
}

bool ServerMux::Route::match(const Request& req) const {
	(void)req;
	return true;
}

// handle registers the handler for the given pattern.
// If a handler already exists for pattern, handle throw exception.
void ServerMux::Route::handle(const std::string& path, const IHandler& handler) {
	(void)path;
	(void)handler;
}

void ServerMux::Route::serve_http(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

IHandler* ServerMux::Route::clone() const {
	return (NULL);
}
