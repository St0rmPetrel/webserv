#include <csignal>

#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"
#include "EventManager.hpp"
#include "ClientEvent.hpp"

using namespace server;

bool server::quit_flag = false;

void server::quitSignalHandler(int signum) {
	(void)signum;
	quit_flag = true;
}

Server::Server(const logger::ILogger& log) : _log(log) {
	signal(SIGINT, server::quitSignalHandler);
}

Server::~Server() { }

void Server::serve_http() {
	EventManager event_manager(_log, _opts);

	_log.info("start serve http");
	while (quit_flag == false) {
		ClientEvent* event = event_manager.accept_event();
		if (event == NULL) {
			continue;
		}
		_log.info("received a client event");
		if (event->process() == ClientEvent::finish_event) {
			event_manager.finish_event(event);
		}
	}
	_log.info("received a interrupt signal, close server");
}

void Server::set_up(const Options& opts) {
	this->_opts = opts;
}
