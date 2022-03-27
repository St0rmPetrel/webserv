#include <csignal>

#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"
#include "EventManager.hpp"
#include "ClientEvent.hpp"

using namespace server;

Server::Server(const logger::ILogger& log) : _log(log) { }

Server::~Server() { }

void Server::serve_http() {
	EventManager event_manager(_log, _opts);

	_log.info("serve_http: start");
	for (;;) {
		// waiting for a event
		ClientEvent* event = event_manager.accept_event();
		if (event == NULL) {
			break;
		}
		_log.info("serve_http: received a client event");
		if (event->process() == ClientEvent::finish_event) {
			event_manager.finish_event(event);
		}
	}
	_log.info("serve_http: close server");
}

void Server::set_up(const Options& opts) {
	this->_opts = opts;
}
