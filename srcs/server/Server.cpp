#include <csignal>

#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"
#include "../utils/utils.hpp"
#include "EventManager.hpp"
#include "Event.hpp"

using namespace server;

Server::Server(const logger::Logger &log, const Options& opts)
	: _log(log), _opts(opts), _event_manager(log) {
	for (std::vector<InetAddr>::const_iterator it = _opts.addrs.begin();
			it != _opts.addrs.end(); it++) {
		this->_event_manager.new_listener(*it);
	}
}

Server::~Server() { }

void Server::listen_and_serve() {
	bool loop = true;

	_log.warn("server: listener and serve: start");
	while (loop) {
		// waiting for events
		const std::set<Event*>& events = _event_manager.accept_events();
		// check and process each event
		for (std::set<Event*>::const_iterator it = events.begin(); it != events.end();
				++it) {
			switch ((*it)->type) {
				case Event::terminate :
					loop = false;
					break;
				case Event::listener : {
					int client = _event_manager.accept_client((*it)->sock);
					(void)client;
					_event_manager.finish_event((*it));
					break;
				}
				case Event::client :
					_log.info("event from client !!!");
					break;
			}
		}
	}
	_log.warn("server: listener and serve: end");
}
