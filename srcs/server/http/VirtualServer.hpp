#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include "ServerMux.hpp"

namespace http {
	struct VirtualServer {
		ServerMux             mux;
		std::string           addr;
		std::set<std::string> names;
//		IHandler              bad_request_handler;
	};
};

#endif
