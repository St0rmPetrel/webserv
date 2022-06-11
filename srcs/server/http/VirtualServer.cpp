#include "VirtualServer.hpp"
#include "SimpleHandler.hpp"
#include "ReturnHandler.hpp"
#include "ErrorPageHandler.hpp"

using namespace http;

VirtualServer::VirtualServer(const logger::Logger& log,
		const VirtualServer::Options& opts) : _log(log), _opts(opts), mux(_log) {

	for (std::vector<VirtualServer::Options::Location>::const_iterator it =
			_opts.locations.begin(); it != _opts.locations.end(); ++it) {
		ServerMux::Route& route = mux.new_route();

		route.set_error_handler(ErrorPageHandler(_log, it->error_page_opts));
		switch (it->handler_type) {
		case Options::Location::FileServer:
			route.handle(it->location_match, SimpleHandler(it->root));
			break;
		case Options::Location::Return:
			route.handle(it->location_match, ReturnHandler(_log, it->return_opts));
			break;
		case Options::Location::CGI:
			break;
		}
	}
}

VirtualServer::VirtualServer(const VirtualServer& vs)
	: _log(vs._log), _opts(vs._opts), mux(vs.mux) { }

VirtualServer::~VirtualServer() { }

const unsigned short int& VirtualServer::get_port() const {
	return _opts.port;
}

const std::string& VirtualServer::get_addr() const {
	return _opts.addr;
}

const std::set<std::string>& VirtualServer::get_names() const {
	return _opts.names;
}

const std::vector<VirtualServer::Options::Location>& VirtualServer::get_locations() const {
	return _opts.locations;
}

VirtualServer::Options::Options() : port(8081), listener_backlog(1024) { }
