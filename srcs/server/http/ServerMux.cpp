#include <string>

#include "ServerMux.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "IHandler.hpp"

using namespace http;

ServerMux::ServerMux()
	: _bad_request_handler(NULL),
		_not_found_handler(NULL),
		_method_not_allowed_handler(NULL)
{ }

ServerMux::ServerMux(const ServerMux& mux)
	: _bad_request_handler(NULL),
		_not_found_handler(NULL),
		_method_not_allowed_handler(NULL)
{
	// copy all handlers
	if (mux._bad_request_handler != NULL) {
		this->_bad_request_handler = mux._bad_request_handler->clone();
	}
	if (mux._not_found_handler != NULL) {
		this->_not_found_handler = mux._not_found_handler->clone();
	}
	if (mux._method_not_allowed_handler != NULL) {
		this->_method_not_allowed_handler = mux._method_not_allowed_handler->clone();
	}
	// copy all routes
	for (std::map<std::string, ServerMux::Route*>::const_iterator it = mux._routes.begin();
			it != mux._routes.end(); ++it) {
		this->_routes[it->first] = dynamic_cast<Route*>(it->second->clone());
	}
	// copy all middlewares
	for (std::vector<IHandler*>::const_iterator it = mux._middlewares.begin();
			it != mux._middlewares.end(); ++it) {
		this->_middlewares.push_back((*it)->clone());
	}
}

ServerMux::~ServerMux() {
	// delete all handlers
	delete _bad_request_handler;
	delete _not_found_handler;
	delete _method_not_allowed_handler;
	// delete all routes
	for (std::map<std::string, ServerMux::Route*>::const_iterator it = _routes.begin();
			it != _routes.end(); ++it) {
		delete it->second;
	}
	// delete all middlewares
	for (std::vector<IHandler*>::const_iterator it = _middlewares.begin();
			it != _middlewares.end(); ++it) {
		delete *it;
	}
}

void ServerMux::serve_http(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

IHandler* ServerMux::clone() const {
	return (new ServerMux(*this));
}

void ServerMux::set_bad_request(const IHandler& bad_request_handler) {
	if (_bad_request_handler != NULL) {
		throw "bad request handler already set";
	}
	_bad_request_handler = bad_request_handler.clone();
}

void ServerMux::bad_request(Response& res, const Request& req) const {
	if (_bad_request_handler != NULL) {
		_bad_request_handler->serve_http(res, req);
		return;
	}
	// default
	(void)res;
	(void)req;
}

void ServerMux::set_not_found(const IHandler& not_found_handler) {
	if (_not_found_handler != NULL) {
		throw "not found handler already set";
	}
	_not_found_handler = not_found_handler.clone();
}

void ServerMux::not_found(Response& res, const Request& req) const {
	if (_not_found_handler != NULL) {
		_not_found_handler->serve_http(res, req);
		return;
	}
	// default
	(void)res;
	(void)req;
}

void ServerMux::set_method_not_allowed(const IHandler& method_not_allowed_handler) {
	if (_method_not_allowed_handler != NULL) {
		throw "method not allowed handler already set";
	}
	_method_not_allowed_handler = method_not_allowed_handler.clone();
}

void ServerMux::method_not_allowed(Response& res, const Request& req) const {
	if (_method_not_allowed_handler != NULL) {
		_method_not_allowed_handler->serve_http(res, req);
		return;
	}
	// default
	(void)res;
	(void)req;
}

ServerMux::Route* ServerMux::new_route() {
	return (new ServerMux::Route(*this));
}

void ServerMux::add_middleware(const IHandler& handler) {
	this->_middlewares.push_back(handler.clone());
}

ServerMux::Route::Route(ServerMux& mux) : _mux(mux) { }

ServerMux::Route::Route(const ServerMux::Route& r) : _mux(r._mux) { }

ServerMux::Route::~Route() { }

ServerMux::Route& ServerMux::Route::method(const std::string& method) {
	this->_allow_methods.insert(method);
	return (*this);
}

ServerMux::Route& ServerMux::Route::host(const std::string& host) {
	this->_allow_hosts.insert(host);
	return (*this);
}

ServerMux::Route& ServerMux::Route::header(const std::pair<std::string, std::string>& pair) {
	this->_mandatory_headers.insert(pair);
	return (*this);
}

bool ServerMux::Route::match(const Request& req) const {
	(void)req;
	return true;
}

// handle registers the handler for the given pattern.
// If a handler already exists for pattern, handle throw exception.
void ServerMux::Route::handle(const std::string& path, const IHandler& handler) {
	if (this->_handler != NULL) {
		throw "handler already bind to route";
	}
	this->_handler = handler.clone();
	if (_mux._routes.find(path) != _mux._routes.end()) {
		throw "path already bind to another route";
	}
	this->_mux._routes[path] = this;
}

void ServerMux::Route::serve_http(Response& res, const Request& req) const {
	(void)res;
	(void)req;
}

IHandler* ServerMux::Route::clone() const {
	return (NULL);
}
