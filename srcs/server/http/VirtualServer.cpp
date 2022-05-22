#include "VirtualServer.hpp"

#include "SimpleHandler.hpp"

using namespace http;

VirtualServer::VirtualServer(const VirtualServer::Options& opts) : opts(opts) {
	for (std::vector<VirtualServer::Options::Location>::const_iterator it = 
			opts.locations.begin(); it != opts.locations.end(); ++it) {
		mux.new_route().handle(it->location_match, SimpleHandler());
	}
}

VirtualServer::VirtualServer(const VirtualServer& vs) : mux(vs.mux), opts(vs.opts) { }

VirtualServer::~VirtualServer() { }
