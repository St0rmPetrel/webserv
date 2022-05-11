#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <stdlib.h>

namespace http {
	struct Request {
		bool        close;
		std::string host;
		std::string method;
		std::string path;
		std::string query;
		std::string body;
		std::string version;
		std::map<std::string, std::string> headers;
	};
};

#endif