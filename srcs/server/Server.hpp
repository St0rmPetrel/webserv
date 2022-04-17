#ifndef SERVER_HPP
# define SERVER_HPP

#include "Options.hpp"
#include "EventManager.hpp"
#include "../logger/Logger.hpp"

#include "http/VirtualServer.hpp"
#include "http/Response.hpp"

namespace server {
	class Server {
		private:
			const logger::Logger& _log;
			Options	              _opts;

			EventManager          _event_manager;

			// TODO debug
			http::VirtualServer   _debug_virtual_server;
		public:
			Server(const logger::Logger &log, const Options& opts);
			~Server();

			// listen_and_serve central method of a Server class which is listen and serve
			// http connection
			void listen_and_serve();
		private:
			int _finish_request(int client_sock, http::Response res);
			const http::VirtualServer& _get_client_virtual_server(int client_sock,
					http::Request req);
	};
};

#endif
