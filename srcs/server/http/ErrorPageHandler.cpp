#include "ErrorPageHandler.hpp"

using namespace http;

ErrorPageHandler::ErrorPageHandler(const logger::Logger& log, const Options& opts)
	: _log(log) {
	for (std::map<Response::StatusCode, std::string>::const_iterator it =
			opts.pages_path.begin(); it != opts.pages_path.end(); ++it) {
		ErrorPageHandler::ErrorPage page;
		// detect page mime_type
		page.mime_type = utils::detect_file_mime_type(it->second);
		// read file
		std::ifstream file(it->second);
		std::stringstream buffer;
		buffer << file.rdbuf();
		page.body = buffer.str();
		// save data in _pages
		_pages[it->first] = page;
	}
}

ErrorPageHandler::ErrorPageHandler(const ErrorPageHandler& ref)
	: _log(ref._log), _pages(ref._pages) { }

ErrorPageHandler::~ErrorPageHandler() { }

void ErrorPageHandler::serve_http(Response& res, const Request& req) const {
	(void)req;
	if (res.is_body()) {
		return ;
	}
	if (_pages.find(res.get_status_code()) == _pages.end()) {
		return ;
	}
	const ErrorPageHandler::ErrorPage& page = _pages(res.get_status_code());
	res.write(page.body, page.mime_type);
}

IHandler* ErrorPageHandler::clone() const {
	return new ErrorPageHandler(*this);
}
