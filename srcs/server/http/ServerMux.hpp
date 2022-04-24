#ifndef SERVER_MUX_HPP
# define SERVER_MUX_HPP

#include <string>

#include "IHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace http {
	class ServerMux : public IHandler {
		public:
			void serve_http(Response& res, const Request& req) const;
			void bad_request(Response& res, const Request& req) const;
			void not_found(Response& res, const Request& req) const;

			// handle registers the handler for the given pattern.
			// If a handler already exists for pattern, handle throw exception.
			void handle(const std::string& pattern, const IHandler& handler);
	};
};

#endif
