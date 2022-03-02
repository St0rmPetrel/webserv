#ifndef ISERVER_HPP
# define ISERVER_HPP

#include "Options.hpp"

namespace server {
	class IServer {
		public:
			virtual ~IServer() { }

			// set_up server according to cofiguration
			virtual void set_up(const Options& opts) = 0;

			// serve_http major function of the project
			// when it called program start to listen address and port in config file and serve
			// http request from clients
			virtual void serve_http() = 0;
	};
}; /* namespace server */

#endif
