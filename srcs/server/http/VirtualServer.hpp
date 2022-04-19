#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include <string>
#include <set>

#include "ServerMux.hpp"

namespace http {
	class VirtualServer {
		public:
			struct Options {
				unsigned short int    port; // Номер порта
				std::string           addr; // IP-адрес
				int                   listener_backlog; // Размер очереди запросов
				std::set<std::string> names; // Имена виртуального сервера
			};
		public:
			VirtualServer(const Options& opts);
			VirtualServer();
			ServerMux mux;
			Options   opts;
	};
};

#endif
