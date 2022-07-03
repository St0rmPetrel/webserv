#ifndef SERVER_OPTIONS_HPP
# define SERVER_OPTIONS_HPP

#include <vector>

#include "http/VirtualServer.hpp"

namespace server {
	struct Options {
		Options();

		unsigned int  recv_buffer_size;
		unsigned long request_body_size_limit;

		std::vector<http::VirtualServer::Options> servers;
	};
}; /* namespace server */

#endif
