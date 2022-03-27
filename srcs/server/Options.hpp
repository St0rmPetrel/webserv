#ifndef SERVER_OPTIONS_HPP
# define SERVER_OPTIONS_HPP

#include <string>
#include <vector>

namespace server {
	struct InetAddr {
		unsigned short int port;    // Номер порта
		std::string        addr;    // IP-адрес
		int                listener_backlog; // Размер очереди запросов
	};

	struct Options {
		int buffer_size;
		int milliseconds_timeout;
		std::vector<InetAddr> addrs;
	};
}; /* namespace server */

#endif
