#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>

namespace http {
	struct Request {
		bool        close;
		std::string host;
		std::string path;
		std::string method;
		std::map<std::string, std::string> header;
	};
};

#endif
