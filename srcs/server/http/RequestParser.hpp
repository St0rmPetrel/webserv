#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include <string>

#include "Request.hpp"

namespace http {
	class RequestParser {
		public:
			enum Result {
				ParsingCompleted,
				ParsingIncompleted,
				ParsingError
			};
		public:
			Result parse(Request& req, const char* begin, const char* end);
	};
};

#endif
