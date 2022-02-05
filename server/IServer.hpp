#ifndef ISERVER_HPP
# define ISERVER_HPP

namespace server {
	class IServer {
		public:
			virtual ~IServer() { }

			// serve_http major function of the project
			// when it called program start to listen address and port in config file and serve
			// http request from clients
			virtual void serve_http() = 0;
	};
}; /* namespace server */

#endif
