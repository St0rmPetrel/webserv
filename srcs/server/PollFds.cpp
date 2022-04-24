#include <unistd.h>
#include <poll.h>

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
// at first close clients than listeners
PollFds::~PollFds() {
	for (std::vector<struct pollfd>::reverse_iterator rit = _base.rbegin();
			rit != _base.rend(); ++rit) {
		close(rit->fd);
	}
}

// do_poll poll syscall method wrapper (blocking)
void PollFds::do_poll() {
	if (poll(&(_base.at(0)), _base.size(), -1) < 0) {
		throw PollFds::PollException();
	}
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
// return set for forward compatibility (if term socket will more than one)
const std::set<int>& PollFds::check_term() {
	this->_term_event_socks.clear();

	if (_base.at(0).revents & POLLIN) {
		_base[0].revents = 0;
		this->_term_event_socks.insert(_base[0].fd);
	}
	return (this->_term_event_socks);
}

// check_listeners check actions on listeners sockets
const std::set<int>& PollFds::check_listeners() {
	this->_listener_event_socks.clear();

	for (std::vector<struct pollfd>::iterator it = _base.begin() + 1;
			it != _base.begin() + 1 + _listeners_num; ++it) {
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

// erase_sock delete socket from base and close this socket
void PollFds::erase_sock(int sock) {
	for (std::vector<struct pollfd>::iterator it = _base.begin(); it != _base.end(); it++) {
		if (it->fd == sock) {
			close(it->fd);
			_base.erase(it);
			break;
		}
	}
}

const char* PollFds::PollException::what() const throw() {
	return "poll error";
}
