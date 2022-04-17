#ifndef SERVER_MUX_HPP
# define SERVER_MUX_HPP

#include <string>

#include "IHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace http {
	class ServerMux : public IHandler {
		public:
			void serve_http(Response& res, Request& req) const;
			void handle(const std::string& pattern, const IHandler& handler);
	};
};

#endif
