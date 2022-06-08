#include "VirtualServer.hpp"

#include "SimpleHandler.hpp"

using namespace http;

VirtualServer::VirtualServer(const logger::Logger& log,
		const VirtualServer::Options& opts) : _log(log), _opts(opts), mux(_log) {

	for (std::vector<VirtualServer::Options::Location>::const_iterator it =
			_opts.locations.begin(); it != _opts.locations.end(); ++it) {
		mux.new_route().handle(it->location_match, SimpleHandler(it->root));
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
