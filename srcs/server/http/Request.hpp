#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>

namespace http {
	struct Request {
		bool        close;
		std::string host;
	};
};

#endif
