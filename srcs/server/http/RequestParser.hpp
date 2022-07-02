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
		private:
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
		public:
			RequestParser(const unsigned long body_size_limit);
			RequestParser(const RequestParser& ref);
			~RequestParser();

			Result parse(Request& req, const char* begin, const char* end);
		private:
			// fill http request based on given raw data
			inline bool isControl(int c);
			inline bool checkVersion(std::string &version);
			void   findQuery(Request& req);
			RequestParser::Result parsingBody(Request& req, std::string &body);
			RequestParser::Result checkHeaders(Request& req, std::string &body, const char *begin_body, const char *end);
			RequestParser::Result parsingChunkedBody(Request& req, const char *begin_body, const char *end);
			void    skipSpaces(Request& req);
		private:
			unsigned long       content_length;
			const unsigned long body_size_limit;
	};
};

#endif
