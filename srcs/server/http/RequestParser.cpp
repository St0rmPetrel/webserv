#include "RequestParser.hpp"
#include "Request.hpp"

using namespace http;

RequestParser::Result RequestParser::parse(Request& req, const char* begin, const char* end) {
	req.close = true;
	(void)begin;
	(void)end;
	return ParsingCompleted;
}
