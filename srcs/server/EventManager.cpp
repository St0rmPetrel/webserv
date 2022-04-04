#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

#include "EventManager.hpp"
#include "../utils/utils.hpp"

using namespace server;

// TODO make custom exception
// EventManager constructor of EventManager open all listeners socket and
// allocate event tree and class of file descriptor warden - PollFds
EventManager::EventManager(const logger::ILogger& log, const Options& opts)
	: _log(log), _opts(opts) {
	struct sockaddr_in addr;
	int                listener;

	// Create PollFds with standard input action termination
	// For example to terminate program press enter
	this->_fds = new PollFds(STDIN);

	// Create and bind listeners socket and put them in PollFds
	// PollFds - file descriptor warden for poll
	for (std::vector<InetAddr>::const_iterator it = _opts.addrs.begin();
			it != _opts.addrs.end(); it++) {
		// create IP/TCP socket
		listener = socket(AF_INET, SOCK_STREAM, 0);
		if (listener < 0) {
			throw std::runtime_error("can't create a listener socket");
		}
		// make socket non blocking for poll
		fcntl(listener, F_SETFL, O_NONBLOCK);
		// bind and then start to listen the socket
		addr.sin_family = AF_INET;
		addr.sin_port = htons(it->port);
		addr.sin_addr.s_addr = inet_addr((const char *)&(it->addr[0]));
		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			throw std::runtime_error("can't bind listener socket");
		}
		if (listen(listener, it->listener_backlog) < 0) {
			throw std::runtime_error("can't listen listener socket");
		}
		// add new listener in to PollFds
		this->_fds->add_listener(listener);
		_log.info(SSTR("event_manager: open listener on " << it->addr << ":" <<
					it->port << " with sock=" << listener));
	}
}

// ~EventManager destructor of EventManager, close all open socket in _fds,
// and delete all client events in _events
EventManager::~EventManager() {
	delete this->_fds;
	for (std::map<int, ClientEvent*>::const_iterator it = _events.begin(); it != _events.end();
			++it) {
		delete it->second;
	}
}


// accept_events main method of class EventManager - defines active sockets
// 1) if this socket of some listener - create new client event and accept new socket for it
// 2) if this is client socket, add event according this socket to return active events set
// 3) if this is termination socket add NULL to return active events set
const std::set<ClientEvent*>& EventManager::accept_events() {
	int           sock;

	_log.debug("start waiting for an event");
	this->_active_events.clear();
	// wait some action in poll
	if (poll(_fds->get_array(), _fds->get_array_size(), -1) < 0) {
		throw std::runtime_error("error in poll");
	}
	// check termination file descriptor actions
	if (_fds->check_term()) {
		_log.debug("receive a terminate action");
		this->_active_events.insert(NULL);
		return (this->_active_events);
	}
	// check clients actions
	const std::set<int>& active_clients = _fds->check_clients();
	for (std::set<int>::const_iterator it = active_clients.begin();
			it != active_clients.end(); ++it) {
		_log.debug("receive client action");
		this->_active_events.insert(this->_events[*it]);
	}
	// check listeners actions
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

// finish_event finish client event - close according client socket and delete event
// from _events
void EventManager::finish_event(ClientEvent* event) {
	_log.debug(SSTR("finish client event: sock=" << event->sock ));
	// close socket and delete it from file descriptor warden PollFds
	this->_fds->erase_client(event->sock);
	// delete event from events tree and from heap (all events is allocated on a heap)
	this->_events.erase(event->sock);
	delete event;
}
