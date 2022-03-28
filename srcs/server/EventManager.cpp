#include <stdexcept>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/signalfd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

#include "EventManager.hpp"
#include "../utils/utils.hpp"

using namespace server;

// TODO make custom exception
EventManager::EventManager(const logger::ILogger& log, const Options& opts)
	: _log(log), _opts(opts) {
	sigset_t           sigset;

	struct sockaddr_in addr;
	int                listener, signal_fd;

	// Create signal_fd for tracking graceful close server event in poll
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGTERM);
	sigprocmask(SIG_SETMASK, &sigset, NULL);

	// Create a new signal file descriptor without flags
	if ((signal_fd = signalfd(-1, &sigset, 0)) < 0) {
		throw std::runtime_error("can't create signal file descriptor");
	}
	fcntl(signal_fd, F_SETFL, O_NONBLOCK);

	this->_fds = new PollFds(signal_fd);

	// Create and bind listeners socket and put them in fds vector for poll
	for (std::vector<InetAddr>::const_iterator it = _opts.addrs.begin();
			it != _opts.addrs.end(); it++) {
		listener = socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0) {
			throw std::runtime_error("can't create a listener socket");
		}
		fcntl(listener, F_SETFL, O_NONBLOCK);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(it->port);
		addr.sin_addr.s_addr = inet_addr((const char *)&(it->addr[0]));
		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			throw std::runtime_error("can't bind listener socket");
		}
		if (listen(listener, it->listener_backlog) < 0) {
			throw std::runtime_error("can't listen listener socket");
		}
		this->_fds->add_listener(listener);
		_log.info(SSTR("open listener on " << it->addr << ":" << it->port));
	}
	_log.debug("construct event manager: fill poll_fds by listeners sockets");
}

EventManager::~EventManager() {
	delete this->_fds;
	for (std::map<int, ClientEvent*>::const_iterator it = _events.begin(); it != _events.end();
			++it) {
		delete it->second;
	}
	_log.debug("destruct event manager: close all sockets in poll_fds");
}

const std::set<ClientEvent*>& EventManager::accept_events() {
	int           sock;

	_log.debug("start waiting for an event");
	this->_active_events.clear();
	// wait some action in poll
	if (poll(_fds->get_array(), _fds->get_array_size(), -1) < 0) {
		throw std::runtime_error("error in poll");
	}
	// check exit signal
	if (_fds->check_term_signal()) {
		_log.debug("receive a terminate signal, gracefully close server");
		this->_active_events.insert(NULL);
		return (this->_active_events);
	}
	// check clients
	const std::set<int>& active_clients = _fds->check_clients();
	for (std::set<int>::const_iterator it = active_clients.begin();
			it != active_clients.end(); ++it) {
		_log.debug("receive client action");
		this->_active_events.insert(this->_events[*it]);
	}
	// check listeners
	const std::set<int>& active_listeners = _fds->check_listeners();
	for (std::set<int>::const_iterator it = active_listeners.begin();
		it != active_listeners.end(); ++it) {

		_log.debug("receive listener action");
		if ((sock = accept(*it, NULL, NULL)) < 0) {
			throw std::runtime_error("can't accept new request");
		}
		if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
			throw std::runtime_error("fcntl error");
		}
		ClientEvent* event = new ClientEvent(sock, _log, _opts);
		this->_events[sock] = event;
		this->_fds->add_client(sock);
	}
	return (this->_active_events);
}

void EventManager::finish_event(ClientEvent* event) {
	_log.debug(SSTR("finish client event: sock=" << event->sock ));
	this->_fds->erase_client(event->sock);
	close(event->sock);
	this->_events.erase(event->sock);
	delete event;
}
