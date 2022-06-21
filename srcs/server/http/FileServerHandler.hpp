#ifndef FILE_SERVER_HANDLER_HPP
# define FILE_SERVER_HANDLER_HPP

#include <string>
#include <set>

#include "IHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class FileServerHandler : public IHandler {
		public:
			struct Options {
				std::string           root;
				bool                  autoindex;
				std::set<std::string> index;

				Options();
				Options(const Options& ref);
				~Options();
			};
		private:
			const logger::Logger _log;
			const Options        _opts;
		public:
			FileServerHandler(const logger::Logger& log, const Options& opts);
			FileServerHandler(const FileServerHandler& ref);
			~FileServerHandler();

			void serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;

			void bad_request(Response& res) const;
			void not_found(Response& res) const;
			void method_not_allowed(Response& res) const;
			void internal_server_error(Response& res) const;

			static bool path_is_valid(const std::string& path);
		private:
			void get_file(Response& res, const Request& req) const;
			void post_file(Response& res, const Request& req) const;
			void delete_file(Response& res, const Request& req) const;

			void _get_dir_or_file(Response& res, const std::string& path) const;
			void _get_file(Response& res, const std::string& path) const;
			void _get_dir(Response& res, const std::string& path) const;
	}; /* FileServerHandler */
}; /* namespace http */

#endif
