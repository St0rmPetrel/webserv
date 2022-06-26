#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include <map>
#include <string>
#include "IHandler.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class CGIHandler : public IHandler {
		public:
			struct Options {
				std::map<std::string, std::string> extention_to_interpretator_path;
				std::map<std::string, std::string> params;

				Options();
				Options(const Options& ref);
				~Options();
			};
		private:
			const logger::Logger _log;
			const Options        _opts;
		public:
			CGIHandler(const logger::Logger& log, const Options& opts);
			CGIHandler(const CGIHandler& ref);
			~CGIHandler();

			void serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;

			void bad_request(Response& res) const;
			void not_found(Response& res) const;
			void internal_server_error(Response& res) const;
	}; /* class CGIHandler */
}; /* namespace http */

#endif
