#include "Server.hpp"
#include "../logger/ILogger.hpp"
#include "../config/IConfig.hpp"

using namespace server;

Server::Server(const logger::ILogger& log) : _log(log) { }

Server::~Server() { }

void Server::serve_http() {
	_log.info("start serve http on port bla bla");
}

void Server::set_up(const config::IServerConfig& conf) {
	(void)conf;
}
