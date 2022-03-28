#include <unistd.h>

#include "PollFds.hpp"

using namespace server;

PollFds::PollFds(int term_signal_fd) : _listeners_num(0) {
	struct pollfd fd;

	fd.fd = term_signal_fd;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.push_back(fd);
}

PollFds::~PollFds() {
	for (std::vector<struct pollfd>::iterator it = _base.begin(); it != _base.end(); it++) {
		close(it->fd);
	}
}

struct pollfd* PollFds::get_array() {
	return &(_base[0]);
}

int PollFds::get_array_size() {
	return _base.size();
}

void PollFds::add_listener(int listener) {
	struct pollfd fd;

	std::vector<struct pollfd>::iterator last_listener = _base.begin() + _listeners_num;

	fd.fd = listener;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.insert(last_listener + 1, fd);
	++(this->_listeners_num);
}

void PollFds::add_client(int sock) {
	struct pollfd fd;

	fd.fd = sock;
	fd.events = POLLIN;
	fd.revents = 0;
	this->_base.push_back(fd);
}

bool PollFds::check_term_signal() {
	if (_base[0].revents & POLLIN) {
		_base[0].revents = 0;
		return true;
	}
	return false;
}

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

void PollFds::erase_client(int sock) {
	for (std::vector<struct pollfd>::iterator it = _base.begin() + 1 + _listeners_num;
			it != _base.end(); it++) {
		if (it->fd == sock) {
			_base.erase(it);
			break;
		}
	}
}
