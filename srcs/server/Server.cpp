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
	bool loop = true;

	_log.info("serve_http: start");
	while (loop) {
		// waiting for events
		const std::set<ClientEvent*>& events = event_manager.accept_events();
		for (std::set<ClientEvent*>::const_iterator it = events.begin(); it != events.end();
				++it) {
			if (*it == NULL) {
				loop = false;
				break;
			}
			_log.info("serve_http: received a client event");
			if ((*it)->process() == ClientEvent::finish_event) {
				event_manager.finish_event(*it);
			}
		}
	}
	_log.info("serve_http: close server");
}

void Server::set_up(const Options& opts) {
	this->_opts = opts;
}
