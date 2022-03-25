#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "EventManager.hpp"
#include "../utils/utils.hpp"

using namespace server;

EventManager::EventManager(const logger::ILogger& log, const Options& opts)
	: _log(log), _opts(opts) {
	struct sockaddr_in addr;
	struct pollfd      fd;
	int                listener;

	_log.debug("initialize event manager, fill _fds for poll by listeners sockets");
	for (std::vector<InetAddr>::const_iterator it = _opts.addrs.begin();
			it != _opts.addrs.end(); it++) {
		listener = socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0) {
			// TODO make custom
			throw std::runtime_error("can't create a listener socket");
		}
		addr.sin_family = AF_INET;
		addr.sin_port = htons(it->port);
		addr.sin_addr.s_addr = inet_addr((const char *)&(it->addr[0]));
		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			// TODO make custom
			throw std::runtime_error("can't bind listener socket");
		}
		this->_listeners.insert(listener);
		fd.fd = listener;
		fd.events = POLLIN;
		this->_fds.push_back(fd);
	}
}

EventManager::~EventManager() {
	_log.debug("delete event manager, close all open sockets");
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++) {
		close(it->fd);
	}
}

ClientEvent* EventManager::accept_event() {
	_log.debug("start waiting for a event, main polling logic");
	// TODO for test
	usleep(1000000);
	return NULL;
}

void EventManager::finish_event(ClientEvent* event) {
	_log.debug(SSTR("delete client with sock=" << event->sock ));
	this->_erase_from_fds(event->sock);
	this->_events.erase(event->sock);
}

void EventManager::_erase_from_fds(int sock) {
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++) {
		if (it->fd == sock) {
			_fds.erase(it);
			break;
		}
	}
}
