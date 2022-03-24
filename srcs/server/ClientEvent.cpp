#include "ClientEvent.hpp"
#include "../utils/utils.hpp"

using namespace server;

ClientEvent::ClientEvent(const int sock, const logger::ILogger& log, const Options& opts)
	: sock(sock), _log(log), _opts(opts) {
	this->_buf = new char[_opts.buffer_size];
	_log.debug(SSTR("create a new client with sock=" << sock));
}

ClientEvent::~ClientEvent() {
	delete[] this->_buf;
	_log.debug(SSTR("delete client with sock=" << sock));
}


ClientEvent::ProcessStatus ClientEvent::process() {
	_log.info(SSTR("start to process client with sock=" << sock));
	return ok;
}
