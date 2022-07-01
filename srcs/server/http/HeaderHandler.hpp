#ifndef HEADER_HANDLER_HPP
# define HEADER_HANDLER_HPP

#include <string>
#include <map>

#include "IHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "../../logger/Logger.hpp"


namespace http {
	class HeaderHandler : public IHandler {
		public:
			struct Options {
				std::map<std::string, std::string> headers;

				Options();
				Options(const Options& ref);
				~Options();
			};
		private:
			const logger::Logger _log;
			const Options        _opts;
		public:
			HeaderHandler(const logger::Logger& log, const Options& opts);
			HeaderHandler(const HeaderHandler& ref);
			~HeaderHandler();

			void serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;
	}; /* class HeaderHandler */
}; /*namespace http */

#endif
