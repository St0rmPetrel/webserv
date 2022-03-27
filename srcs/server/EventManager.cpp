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
	: _log(log), _opts(opts), _listeners_num(0) {
	sigset_t           sigset;

	struct sockaddr_in addr;
	struct pollfd      fd;
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
	fd.fd = signal_fd;
	fd.events = POLLIN;
	this->_fds.push_back(fd);

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
		fd.fd = listener;
		fd.events = POLLIN;
		this->_fds.push_back(fd);
		++_listeners_num;
		_log.info(SSTR("open listener on " << it->addr << ":" << it->port));
	}
	_log.debug("construct event manager: fill poll_fds by listeners sockets");
}

EventManager::~EventManager() {
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++) {
		close(it->fd);
	}
	_log.debug("destruct event manager: close all sockets in poll_fds");
}

ClientEvent* EventManager::accept_event() {
	int           sock;

	_log.debug(SSTR("start waiting for a event, main polling logic fds.size()=" << _fds.size()));
	for (;;) {
		_log.debug("start new poll loop");
		// wait some action in poll
		if (poll(&(_fds[0]), _fds.size(), -1) < 0) {
			throw std::runtime_error("error in poll");
		}
		// check exit signal
		if (_fds[0].revents & POLLIN) {
			_fds[0].revents = 0;
			_log.debug("receive a terminate signal, gracefully close server");
			return NULL;
		}
		// check open clients
		for (std::vector<struct pollfd>::iterator it = _fds.begin() + 1 + _listeners_num;
				it != _fds.end(); it++) {
			if (it->revents & POLLIN) {
				_log.debug("receive client action");
				it->revents = 0;
				return _events[it->fd];
			}
		}
		// check listeners
		for (std::vector<struct pollfd>::iterator it = _fds.begin() + 1;
				it != _fds.begin() + 1 + _listeners_num; it++) {
			if (it->revents & POLLIN) {
				_log.debug("receive listener action");
				it->revents = 0;
				if ((sock = accept(it->fd, NULL, NULL)) < 0) {
					throw std::runtime_error("can't accept new request");
				}
				fcntl(sock, F_SETFL, O_NONBLOCK);
				ClientEvent* event = new ClientEvent(sock, _log, _opts);
				_events[sock] = event;
				_add_to_client_fds(sock);
				// TODO make better add_to_client_fds
				break;
			}
		}
	}
}

void EventManager::finish_event(ClientEvent* event) {
	_log.debug(SSTR("finish client event: sock=" << event->sock ));
	this->_erase_from_client_fds(event->sock);
	close(event->sock);
	this->_events.erase(event->sock);
}

void EventManager::_erase_from_client_fds(int sock) {
	for (std::vector<struct pollfd>::iterator it = _fds.begin() + 1 + _listeners_num;
			it != _fds.end(); it++) {
		if (it->fd == sock) {
			_fds.erase(it);
			break;
		}
	}
}

void EventManager::_add_to_client_fds(int sock) {
	struct pollfd fd;

	fd.fd = sock;
	fd.events = POLLIN;
	this->_fds.push_back(fd);
}
