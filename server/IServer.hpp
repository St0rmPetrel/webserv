#ifndef ISERVER_HPP
# define ISERVER_HPP

#include "../config/Config.hpp"
#include "../logger/Logger.hpp"

namespace server {
	class IServer {
		public:
			virtual ~IServer() { }

			virtual void set_up(const config::IServerConfig& conf) = 0;

			// serve_http major function of the project
			// when it called program start to listen address and port in config file and serve
			// http request from clients
			virtual void serve_http() = 0;
	};
}; /* namespace server */

#endif
