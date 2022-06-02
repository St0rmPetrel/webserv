#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>

namespace http {
	const std::string method_get = "GET";
	const std::string method_put = "PUT";
	const std::string method_delete = "DELETE";
	const std::string method_post = "POST";

	struct Request {
		bool                               close;
		std::string                        host;
		std::string                        method;
		std::string                        path;
		std::string                        query;
		std::string                        body;
		std::string                        version;
		std::map<std::string, std::string> headers;
	};
};

#endif
