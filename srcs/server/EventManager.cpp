#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "EventManager.hpp"
#include "../utils/utils.hpp"

using namespace server;

// EventManager constructor of EventManager
// create event tree and descriptor warden class - PollFds initialized by STDIN file
// descriptor for termination event
EventManager::EventManager(const logger::ILogger& log) : _log(log), _fds(STDIN) { }

// ~EventManager destructor of EventManager, close all open socket in _fds (_fds destructor),
// and delete all client events in _events
EventManager::~EventManager() {
	for (std::map<int, Event*>::const_iterator it = _events.begin(); it != _events.end();
			++it) {
		delete it->second;
	}
}

// new_listener creates and binds listener socket
// and add it in _fds listeners
int EventManager::new_listener(const InetAddr& iaddr) {
	int                listener;
	struct sockaddr_in addr;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		throw EventManager::OpenSocketException();
	}
	// make socket non blocking for poll
	if (fcntl(listener, F_SETFL, O_NONBLOCK) < 0) {
		throw EventManager::FcntlSocketException();
	}
	// bind and then start to listen the socket
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iaddr.port);
	addr.sin_addr.s_addr = inet_addr((const char *)&(iaddr.addr[0]));
	if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		throw EventManager::BindSocketException();
	}
	if (listen(listener, iaddr.listener_backlog) < 0) {
		throw EventManager::ListenSocketException();
	}
	// add new listener in to PollFds
	this->_fds.add_listener(listener);
	_log.info(SSTR("event_manager: open listener on " << iaddr.addr << ":" <<
				iaddr.port << " with sock=" << listener));
	return listener;
}

// accept_client accept client from listener and add in _fds clients
int EventManager::accept_client(int listener) {
	int client;

	if ((client = accept(listener, NULL, NULL)) < 0) {
		throw EventManager::ListenerAcceptException();
	}
	if (fcntl(client, F_SETFL, O_NONBLOCK) < 0) {
		throw EventManager::FcntlSocketException();
	}
	this->_fds.add_client(client);
	return client;
}


// _get_event create new event or return exist in _events map
Event* EventManager::_get_event(int sock, Event::Type type) {
	Event* event;

	if (_events.find(sock) != _events.end()) {
		return _events[sock];
	}
	event = new Event;
	event->sock = sock;
	event->type = type;
	_events[sock] = event;
	return event;
}

// accept_events main method of class EventManager - defines active sockets and type this sockets
const std::set<Event*>& EventManager::accept_events() {
	Event* event;

	_log.debug("start waiting for an event");
	this->_active_events.clear();
	// wait some action in poll
	_fds.do_poll();
	// check termination file descriptor actions
	const std::set<int>& active_term = _fds.check_term();
	for (std::set<int>::const_iterator it = active_term.begin();
			it != active_term.end(); ++it) {
		_log.debug("receive a termination action");
		event = this->_get_event(*it, Event::terminate);
		this->_active_events.insert(event);
		return (this->_active_events);
	}
	// check clients actions
	const std::set<int>& active_clients = _fds.check_clients();
	for (std::set<int>::const_iterator it = active_clients.begin();
			it != active_clients.end(); ++it) {
		_log.debug("receive client action");
		event = this->_get_event(*it, Event::client);
		this->_active_events.insert(event);
	}
	// check listeners actions
	const std::set<int>& active_listeners = _fds.check_listeners();
	for (std::set<int>::const_iterator it = active_listeners.begin();
			it != active_listeners.end(); ++it) {
		_log.debug("receive listener action");
		event = this->_get_event(*it, Event::listener);
		this->_active_events.insert(event);
	}
	return (this->_active_events);
}

// finish_event finish client event - close according client socket and delete event
// from _events
void EventManager::finish_event(Event* event) {
	_log.debug(SSTR("finish event: sock=" << event->sock ));
	// close socket and delete it from file descriptor warden PollFds
	this->_fds.erase_sock(event->sock);
	// delete event from events tree and from heap (all events is allocated on a heap)
	this->_events.erase(event->sock);
	delete event;
}

const char* EventManager::OpenSocketException::what() const throw() {
	return "fail to open IP/TCP socket";
}

const char* EventManager::FcntlSocketException::what() const throw() {
	return "fail to set socket in non blocking mode";
}

const char* EventManager::BindSocketException::what() const throw() {
	return "fail to bind socket to address";
}

const char* EventManager::ListenSocketException::what() const throw() {
	return "fail to set socket in listen mode";
}

const char* EventManager::ListenerAcceptException::what() const throw() {
	return "fail to accept socket from listener";
}
