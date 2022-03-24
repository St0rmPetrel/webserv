#include "EventManager.hpp"
#include "../utils/utils.hpp"

using namespace server;

EventManager::EventManager(const std::vector<int>& listeners,
					const logger::ILogger& log, const Options& opts)
	: _listeners(listeners), _log(log), _opts(opts) {
	_log.debug("initialize event manager, fill _fds for poll by listeners sockets");
	(void)_listeners;
}

EventManager::~EventManager() {
	_log.debug("delete event manager, close all open sockets");
}

ClientEvent& EventManager::accept_event() {
	_log.debug("start waiting for a event, main polling logic");
	// TODO just for compiling
	ClientEvent* e = new ClientEvent(0, _log, _opts);
	return *e;
}

void EventManager::finish_event(ClientEvent& event) {
	_log.debug(SSTR("delete client with sock=" << event.sock ));
	// TODO delete sock from _fds (no need to close)
	this->_events.erase(event.sock);
}
