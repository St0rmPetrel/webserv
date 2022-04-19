#ifndef SERVER_HPP
# define SERVER_HPP

#include "Options.hpp"
#include "EventManager.hpp"
#include "../logger/Logger.hpp"

#include "http/VirtualServer.hpp"
#include "http/Response.hpp"
#include "http/Request.hpp"
#include "http/RequestParser.hpp"

namespace server {
	class Server {
		private:
			const logger::Logger& _log;
			Options	              _opts;

			EventManager          _event_manager;
			http::RequestParser   _request_parser;

			std::map<int, int>                               _clients_listener;
			std::map<int, http::Request>                     _clients_request;
			std::map<int, std::vector<http::VirtualServer> > _listeners_virtual_servers;
		public:
			Server(const logger::Logger &log, const Options& opts);
			~Server();

			// listen_and_serve central method of a Server class which is listen and serve
			// http connection
			void listen_and_serve();
		private:
			int _finish_request(int client_sock, const http::Response& res);
			const http::VirtualServer& _get_client_virtual_server(int client_sock,
					const http::Request& req);
			int _find_listener(const std::string& addr, unsigned short int port);
	};
};

#endif
