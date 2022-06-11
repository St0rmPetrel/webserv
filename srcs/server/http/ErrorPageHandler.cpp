#include <fstream>

#include "ErrorPageHandler.hpp"
#include "../../utils/utils.hpp"

using namespace http;

ErrorPageHandler::ErrorPageHandler(const logger::Logger& log, const Options& opts)
	: _log(log) {
	for (std::map<Response::StatusCode, std::string>::const_iterator it =
			opts.pages_path.begin(); it != opts.pages_path.end(); ++it) {
		ErrorPageHandler::ErrorPage page;
		// detect page mime_type
		page.mime_type = utils::detect_file_mime_type(it->second);
		// read file
		std::ifstream ifs;
		ifs.open(it->second.c_str(), std::ifstream::in);
		if (ifs.fail()) {
			_log.fatal(SSTR(
					"[ErrorPageHandler] [constructor] fail to open file with path: " <<
					it->second));
			throw FailOpenFileException();
		}
		std::stringstream buffer;
		buffer << ifs.rdbuf();
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
	std::map<Response::StatusCode, ErrorPageHandler::ErrorPage>::const_iterator
		error_page = _pages.find(res.get_status_code());
	if (error_page == _pages.end()) {
		return ;
	}
	const ErrorPageHandler::ErrorPage& page = error_page->second;
	res.write(page.body, page.mime_type);
}

IHandler* ErrorPageHandler::clone() const {
	return new ErrorPageHandler(*this);
}

const char* ErrorPageHandler::FailOpenFileException::what() const throw() {
	return "fail to open file";
}
