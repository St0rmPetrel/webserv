#include "HeaderHandler.hpp"

using namespace http;

HeaderHandler::HeaderHandler(const  logger::Logger& log, const Options& opts)
	: _log(log), _opts(opts) { }

HeaderHandler::HeaderHandler(const HeaderHandler& ref)
	: _log(ref._log), _opts(ref._opts) { }

HeaderHandler::~HeaderHandler() { }

IHandler* HeaderHandler::clone() const {
	return (new HeaderHandler(*this));
}

void HeaderHandler::serve_http(Response& res, const Request& req) const {
	(void)req;
	for (std::map<std::string, std::string>::const_iterator it = _opts.headers.begin();
			it != _opts.headers.end(); ++it) {
		res.header.set(it->first, it->second);
	}
}

HeaderHandler::Options::Options() { }

HeaderHandler::Options::Options(const Options& ref)
	: headers(ref.headers) { }

HeaderHandler::Options::~Options() { }
