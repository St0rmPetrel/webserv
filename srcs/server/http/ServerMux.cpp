#include <string>

#include "ServerMux.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "IHandler.hpp"

using namespace http;

ServerMux::ServerMux(const logger::Logger log)
	: _log(log),
		_new_route(NULL)
{ }

ServerMux::ServerMux(const ServerMux& mux)
	: _log(mux._log),
		_new_route(NULL)
{
	// copy new route
	if (mux._new_route != NULL) {
		this->_new_route = mux._new_route->clone();
	}
	// copy routes
	for (std::vector<const Route*>::const_iterator it = mux._routes.begin();
			it != mux._routes.end(); ++it) {
		this->_routes.push_back(dynamic_cast<const Route*>((*it)->clone()));
	}
}

ServerMux::~ServerMux() {
	// delete new_route
	delete _new_route;
	// delete all routes
	for (std::vector<const Route*>::const_iterator it = _routes.begin();
			it != _routes.end(); ++it) {
		delete *it;
	}
}

// serve_http select match route in priority order
void ServerMux::serve_http(Response& res, const Request& req) const {
	_log.debug("[ServerMux] Hello from server mux");
	for (std::vector<const Route*>::const_iterator it = _routes.begin();
			it != _routes.end(); ++it) {
		if ((*it)->match(req)) {
			(*it)->serve_http(res, req);
			return;
		}
	}
	this->not_found(res);
}

// clone deep copy of ServerMux class on heap
IHandler* ServerMux::clone() const {
	return (new ServerMux(*this));
}

// bad_request fill response with http bad request (400)
void ServerMux::bad_request(Response& res) const {
	res.write_header(Response::BadRequest);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>400 Bad Request</h1>\n"
		"  <p>This page isn't work.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

// not_found fill response with http not found (404)
void ServerMux::not_found(Response& res) const {
	res.write_header(Response::NotFound);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>404 Not Found</h1>\n"
		"  <p>The requested URL was not found on server.</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

// method_not_allowed fill response with http method not allowed (405)
void ServerMux::method_not_allowed(Response& res) const {
	res.write_header(Response::MethodNotAllowed);
	std::string body = "<html>\n"
		" <body>\n"
		"  <h1>405 Not Allowed</h1>\n"
		"  <p>webserv</p>\n"
		" </body>\n"
		"</html>\n";
	res.write(body, http::mime_type_html);
}

// new_route create new route for set it and than handle (bind to ServerMux)
ServerMux::Route& ServerMux::new_route() {
	if (_new_route != NULL) {
		delete _new_route;
		_new_route = NULL;
	}
	_new_route = new ServerMux::Route(*this);
	return *(dynamic_cast<Route*>(_new_route));
}

// _add_route add configured route in to ServerMux (bind route to ServerMux)
void ServerMux::_add_route(const std::string& path, const Route* route) {
	for (std::vector<const Route*>::iterator it = _routes.begin();
			it != _routes.end(); ++it) {
		if ((*it)->_path == path) {
			throw ServerMux::BusyPathException();
		}
		if ((*it)->_path.size() <= path.size()) {
			_routes.insert(it, route);
			return;
		}
	}
	_routes.push_back(route);
}

const char* ServerMux::BusyPathException::what() const throw() {
	return "path already bind to another route";
}

const char* ServerMux::HandlerExistException::what() const throw() {
	return "handler already set";
}

ServerMux::Route::Route(ServerMux& mux)
	: _mux(mux)
{ }

ServerMux::Route::Route(const ServerMux::Route& r)
	: _mux(r._mux)
	  , _path(r._path)
	  , _allow_methods(r._allow_methods)
	  , _allow_hosts(r._allow_hosts)
	  , _mandatory_headers(r._mandatory_headers)
{
	for (std::vector<IHandler*>::const_iterator it = r._handler_chain.begin();
			it != r._handler_chain.end(); ++it) {
		_handler_chain.push_back((*it)->clone());
	}
}

ServerMux::Route::~Route() {
	for (std::vector<IHandler*>::const_iterator it = _handler_chain.begin();
			it != _handler_chain.end(); ++it) {
		delete *it;
	}
}

// method add new http allowed method in to route
// if never calls than all http methods is allow
ServerMux::Route& ServerMux::Route::method(const std::string& method) {
	this->_allow_methods.insert(method);
	return (*this);
}

// method add new http allowed host in to route
// if never calls than all http hosts is allow
ServerMux::Route& ServerMux::Route::host(const std::string& host) {
	this->_allow_hosts.insert(host);
	return (*this);
}

// hander add required http header pair (key, value) into route
ServerMux::Route& ServerMux::Route::header(const std::pair<std::string, std::string>& pair) {
	this->_mandatory_headers.insert(pair);
	return (*this);
}

// match check http request that it suit the route
bool ServerMux::Route::match(const Request& req) const {
	// check path
	if (req.path.find(_path) != 0) {
		return false;
	}
	// check host
	if (!_allow_hosts.empty() && _allow_hosts.find(req.host) == _allow_hosts.end()) {
		return false;
	}
	// check method
	if (!_allow_methods.empty() && _allow_methods.find(req.host) == _allow_methods.end()) {
		return false;
	}
	// check headers
	if (!_mandatory_headers.empty()) {
		for (std::map<std::string, std::string>::const_iterator it = req.headers.begin();
				it != req.headers.end(); ++it) {
			if (_mandatory_headers.find(*it) != _mandatory_headers.end()) {
				return true;
			}
		}
		return false;
	}
	return true;
}

void ServerMux::Route::push_back_handler(const IHandler& handler) {
	this->_handler_chain.push_back(handler.clone());
}

void ServerMux::Route::mux_register(const std::string& path) {
	if (_mux._new_route == NULL) {
		throw ServerMux::Route::ExistHandlerException();
	}
	if (_handler_chain.empty()) {
		throw ServerMux::Route::EmptyHandlerException();
	}

	this->_path = path;
	_mux._add_route(path, this);
	_mux._new_route = NULL;
}

// serve_http calls serve_http function of bind to route handler
void ServerMux::Route::serve_http(Response& res, const Request& req) const {
	for (std::vector<IHandler*>::const_iterator it = _handler_chain.begin();
			it != _handler_chain.end(); ++it) {
		(*it)->serve_http(res, req);
	}
}

// clone deep copy on heap a Route
IHandler* ServerMux::Route::clone() const {
	return (new ServerMux::Route(*this));
}

const char* ServerMux::Route::EmptyHandlerException::what() const throw() {
	return "handler not bind to route";
}

const char* ServerMux::Route::ExistHandlerException::what() const throw() {
	return "handler already bind to the route";
}
