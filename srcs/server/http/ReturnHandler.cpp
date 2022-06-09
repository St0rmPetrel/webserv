#include "ReturnHandler.hpp"

using namespace http;

ReturnHandler::ReturnHandler(const logger::Logger& log, const Options& opts)
	: _log(log), _opts(opts) { }

ReturnHandler::ReturnHandler(const ReturnHandler& ref) : _log(ref._log), _opts(ref._opts) { }

ReturnHandler::~ReturnHandler() { }

void ReturnHandler::serve_http(Response& res, const Request& req) const {
	(void)req;
	res.write_header(_opts.code);
	if (is_redirect_code(_opts.code)) {
		res.header.set("Location", _opts.url);
	} else {
		if (!_opts.text.empty()) {
			res.write(_opts.text, http::mime_type_txt);
		}
	}
}

IHandler* ReturnHandler::clone() const {
	return (new ReturnHandler(*this));
}

const char* ReturnHandler::EmptyRedirectURLException::what() const throw() {
	return "return handler with redirect status code must have URL to redirect parameter";
}

ReturnHandler::Options::Options(const Response::StatusCode& code, const std::string& text_or_url)
	: code(code) {
	if (is_redirect_code(code)) {
		if (text_or_url.empty()) {
			throw ReturnHandler::EmptyRedirectURLException();
		}
		url = text_or_url;
	} else {
		text = text_or_url;
	}
}

ReturnHandler::Options::Options(const Options& ref)
	: code(ref.code), url(ref.url), text(ref.text) { }

ReturnHandler::Options::Options() : code(http::Response::OK) { }

ReturnHandler::Options::~Options() { }
