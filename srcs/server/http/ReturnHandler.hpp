#ifndef RETURN_HANDLER_HPP
# define RETURN_HANDLER_HPP

#include <string>

#include "IHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class ReturnHandler : public IHandler {
		public:
			struct Options {
				Response::StatusCode code;
				std::string          url;
				std::string          text;

				Options();
				Options(const Response::StatusCode& code, const std::string& text_or_url = "");
				Options(const Options& ref);

				Options& operator=(const Options& rh);
				~Options();
			};

			struct EmptyRedirectURLException : public std::exception {
				virtual const char* what() const throw();
			};
		private:
			const logger::Logger _log;
			const Options        _opts;
		public:
			ReturnHandler(const logger::Logger& log, const Options& opts);
			ReturnHandler(const ReturnHandler& ref);
			~ReturnHandler();

			void serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;

	}; /* ReturnHandler */
}; /* namespace http */

#endif
