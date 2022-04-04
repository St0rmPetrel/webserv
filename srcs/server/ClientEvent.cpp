#include <sys/socket.h>

#include "ClientEvent.hpp"
#include "../utils/utils.hpp"

using namespace server;

ClientEvent::ClientEvent(const int sock, const logger::ILogger& log, const Options& opts)
	: sock(sock), _log(log), _opts(opts) {
	this->_buf = new char[_opts.buffer_size];
	_log.debug(SSTR("create a new client with sock=" << sock));
}

ClientEvent::ClientEvent(const ClientEvent& src)
	: sock(src.sock), _log(src._log), _opts(src._opts) {
	this->_buf = new char[_opts.buffer_size];
}

ClientEvent::~ClientEvent() {
	delete[] this->_buf;
	_log.debug(SSTR("delete client with sock=" << sock));
}


// receive http request, process it and response
ClientEvent::ProcessStatus ClientEvent::process() {
	_log.info(SSTR("start to process client with sock=" << sock));
	send(this->sock, (void *)"Hello from server!\n", 19, 0);
	return finish_event;
}
