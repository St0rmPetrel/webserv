#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include <string>

#include "Request.hpp"
#include <iostream>
#include <string.h>

namespace http {
	class RequestParser {
		public:
			enum Result {
				ParsingCompleted,
				ParsingIncompleted,
				ParsingError
			};
			enum State {
			    ParseFirstLine,
				RequestPath,
				RequestVersionStart,
				RequestVersion,
				RequestHeaderKey,
				RequestHeaderValueStart,
				RequestHeaderValue,
				RequestBody,
				EndOfHeaders,
				};
			enum BodyState {
			    StartBodyParse,
			    ParseSizeOfChunk,
			    ParseChunk,
			    EndOfChunk,
			    AddChunkToBody
			    };
		unsigned long content_length;
		public:
			Result parse(Request& req, const char* begin, const char* end);
			inline bool isControl(int c);
			inline bool checkVersion(std::string &version);
			void   findQuery(Request& req);
			RequestParser::Result parsingBody(Request& req, std::string &body);
			RequestParser::Result checkHeaders(Request& req, std::string &body, const char *begin_body, const char *end);
			RequestParser::Result parsingChunkedBody(Request& req, const char *begin_body, const char *end);
			void    skipSpaces(Request& req);
	};
};

#endif
