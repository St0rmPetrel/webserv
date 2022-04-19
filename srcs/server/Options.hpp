#ifndef SERVER_OPTIONS_HPP
# define SERVER_OPTIONS_HPP

#include <vector>

#include "http/VirtualServer.hpp"

namespace server {

	struct Options {
		int recv_buffer_size;
		std::vector<http::VirtualServer::Options> servers;
	};
}; /* namespace server */

#endif
