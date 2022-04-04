#include <unistd.h>

#include "PollFds.hpp"

using namespace server;

// PollFds push term_fd to base
PollFds::PollFds(int term_fd) : _listeners_num(0) {
	struct pollfd fd;

	fd.fd = term_fd;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.push_back(fd);
}

// ~PollFds close all socket in _base
PollFds::~PollFds() {
	for (std::vector<struct pollfd>::iterator it = _base.begin(); it != _base.end(); it++) {
		close(it->fd);
	}
}

// get_array give data for poll syscall (second argument in poll)
struct pollfd* PollFds::get_array() {
	return &(_base[0]);
}

// get_array_size give data for poll syscall (second argument in poll)
int PollFds::get_array_size() {
	return _base.size();
}

// add_listener add listener socket in class
void PollFds::add_listener(int listener) {
	struct pollfd fd;

	std::vector<struct pollfd>::iterator last_listener = _base.begin() + _listeners_num;

	fd.fd = listener;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.insert(last_listener + 1, fd);
	++(this->_listeners_num);
}

// add_client add client socket in class
void PollFds::add_client(int sock) {
	struct pollfd fd;

	fd.fd = sock;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.push_back(fd);
}

// check_term check actions on term_fd
bool PollFds::check_term() {
	if (_base[0].revents & POLLIN) {
		_base[0].revents = 0;
		return true;
	}
	return false;
}

// check_listeners check actions on listeners sockets
const std::set<int>& PollFds::check_listeners() {
	this->_listener_event_socks.clear();

	for (std::vector<struct pollfd>::iterator it = _base.begin() + 1;
			it != _base.begin() + 1 + _listeners_num; it++) {
		if (it->revents & POLLIN) {
			it->revents = 0;
			this->_listener_event_socks.insert(it->fd);
		}
	}
	return (this->_listener_event_socks);
}

// check_clients check actions on clients sockets
const std::set<int>& PollFds::check_clients() {
	this->_client_event_socks.clear();

	for (std::vector<struct pollfd>::iterator it = _base.begin() + 1 + _listeners_num;
			it != _base.end(); it++) {
		if (it->revents & POLLIN) {
			it->revents = 0;
			this->_client_event_socks.insert(it->fd);
		}
	}
	return (this->_client_event_socks);
}

// erase_client delete client socket from base and close this socket
void PollFds::erase_client(int sock) {
	for (std::vector<struct pollfd>::iterator it = _base.begin() + 1 + _listeners_num;
			it != _base.end(); it++) {
		if (it->fd == sock) {
			close(it->fd);
			_base.erase(it);
			break;
		}
	}
}
