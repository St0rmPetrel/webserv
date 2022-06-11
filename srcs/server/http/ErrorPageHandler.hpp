#ifndef ERROR_PAGE_HANDLER_HPP
# define ERROR_PAGE_HANDLER_HPP

#include <string>
#include <map>

#include "IHandler.hpp"
#include "Response.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class ErrorPageHandler : public IHandler {
		public:
			struct Options {
				std::map<Response::StatusCode, std::string> pages_path;
			};
			struct ErrorPage {
				std::string body;
				std::string mime_type;
			};

			struct FailOpenFileException : public std::exception {
				virtual const char* what() const throw();
			};
		private:
			const logger::Logger _log;

			std::map<Response::StatusCode, ErrorPage> _pages;
		public:
			ErrorPageHandler(const logger::Logger& log, const Options& opts);
			ErrorPageHandler(const ErrorPageHandler& ref);
			~ErrorPageHandler();

			void serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;
	}; /* class ErrorPageHandler */
}; /* namespace http */

#endif
