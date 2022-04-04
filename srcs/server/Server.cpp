#include <csignal>

#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"
#include "../utils/utils.hpp"
#include "EventManager.hpp"
#include "ClientEvent.hpp"

using namespace server;

Server::Server(const logger::Logger &log) : _log(log) { }

Server::~Server() { }

void Server::serve_http() {
	EventManager event_manager(_log, _opts);
	bool loop = true;

	_log.warn("serve_http: start");
	while (loop) {
		// waiting for events
		const std::set<ClientEvent*>& events = event_manager.accept_events();
		// check and process each event
		for (std::set<ClientEvent*>::const_iterator it = events.begin(); it != events.end();
				++it) {
			if (*it == NULL) {
				// received an action on terminate socket (STDIN = 0 by default)
				loop = false;
				break;
			}
			// received an ordinary client action on some client socket accepted by listener
			_log.info(SSTR("serve_http: received a client event on socket=" << (*it)->sock));
			if ((*it)->process() == ClientEvent::finish_event) {
				// event is finished, the socket is closed and the event is removed from
				// event manager
				event_manager.finish_event(*it);
			}
		}
	}
	_log.warn("serve_http: close server");
}

void Server::set_up(const Options& opts) {
	this->_opts = opts;
}
