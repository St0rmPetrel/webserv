#include "VirtualServer.hpp"
#include "FileServerHandler.hpp"
#include "ReturnHandler.hpp"
#include "ErrorPageHandler.hpp"

using namespace http;

VirtualServer::VirtualServer(const logger::Logger& log,
		const VirtualServer::Options& opts) : _log(log), _opts(opts), mux(_log) {

	for (std::vector<VirtualServer::Options::Location>::const_iterator it =
			_opts.locations.begin(); it != _opts.locations.end(); ++it) {
		ServerMux::Route& route = mux.new_route();

		// set allow methods to route
		for (std::set<std::string>::const_iterator am_it = it->allow_methods.begin();
				am_it != it->allow_methods.end(); ++am_it) {
			route.method(*am_it);
		}

		// set handler chain bound to route
		switch (it->handler_type) {
		case Options::Location::FileServer:
			route.push_back_handler(FileServerHandler(_log, it->file_server_opts));
			break;
		case Options::Location::Return:
			route.push_back_handler(ReturnHandler(_log, it->return_opts));
			break;
		case Options::Location::CGI:
			route.push_back_handler(CGIHandler(_log, it->cgi_opts));
			break;
		}
		route.push_back_handler(ErrorPageHandler(_log, it->error_page_opts));
		route.push_back_handler(HeaderHandler(_log, it->header_opts));
		route.mux_register(it->location_match);
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

bool VirtualServer::Options::intersect(const VirtualServer::Options& opts) const {
	if (opts.port != this->port || opts.addr != this->addr) {
		return false;
	}
	if (opts.names.empty() && this->names.empty()) {
		return true;
	}
	for (std::set<std::string>::const_iterator cit = opts.names.begin();
			cit != opts.names.end(); ++cit) {
		std::set<std::string>::const_iterator name_cit = this->names.find(*cit);
		if (name_cit != this->names.end()) {
			return true;
		}
	}
	return false;
}
