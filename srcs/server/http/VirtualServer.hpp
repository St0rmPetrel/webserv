#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include <string>
#include <vector>
#include <set>
#include <map>

#include "ServerMux.hpp"
#include "Response.hpp"

namespace http {
	class VirtualServer {
		public:
			struct Options {
				struct Location {
					std::string location_match;
					std::string root;
					std::map<Response::StatusCode, std::string> error_page;
				};

				unsigned short int    port; // Номер порта
				std::string           addr; // IP-адрес
				int                   listener_backlog; // Размер очереди запросов
				std::set<std::string> names; // Имена виртуального сервера
				std::vector<Location> locations;

				Options();
			};
		public:
			VirtualServer(const Options& opts);
			VirtualServer(const VirtualServer& vs);
			~VirtualServer();
			ServerMux mux;
			Options   opts;
	};
};

#endif
