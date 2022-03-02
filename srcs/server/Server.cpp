#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"

using namespace server;

Server::Server(logger::Logger log) : _log(log) { }

Server::~Server() { }

void Server::serve_http() {
	_log.info("start serve http on port bla bla");
}

void Server::set_up(const Options& opts) {
	this->_opts = opts;
}
