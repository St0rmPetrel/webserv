#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

#include "RequestParser.hpp"
#include "Request.hpp"

using namespace http;

RequestParser::Result RequestParser::parse(Request& req, const char* begin, const char* end)
{
	req.close = true;
	const char  *head = begin;
	const char  *in = begin;
	std::string   key;
	std::string   value;
	int   i = 0;
	State state = ParseFirstLine;
	std::string   body;

	while (in++ != end)
	{
		if (state == ParseFirstLine)
		{
			if (isControl(*in))
				return ParsingError;
			if (*in == ' ')
			{
				req.method = std::string(head, in);
				if (req.method != method_get && req.method != method_put &&
						req.method != method_delete && req.method != method_post)
					return ParsingError;
				head = in + 1;
				state = RequestPath;
				continue ;
			}
		}

		if (state == RequestPath)
		{
			if (isControl(*in))
				return ParsingError;
			if (*in == ' ')
			{
				req.path = std::string(head, in);
				findQuery(req);
				head = in + 1;
				state = RequestVersionStart;
				continue ;
			}
		}

		if (state == RequestVersionStart)
		{
			if (*in == '\r')
				state = RequestVersion;
			continue ;
		}

		if (state == RequestVersion)
		{
			if (*in == '\n')
			{
				req.version = std::string(head, in - 1);
				if (req.version[i] != 'H' || req.version[i + 1] != 'T' || req.version[i + 2] != 'T'
				|| req.version[i + 3] != 'P' || req.version[i + 4] != '/')
					return ParsingError;
				req.version.erase(i, i + 5);
				if (!checkVersion(req.version))
					return ParsingError;
				head = in + 1;
				state = RequestHeaderKey;
				continue ;
			}
			else
				return ParsingError;
		}

		if (state == RequestHeaderKey)
		{
			if (*in == '\r')
			{
				state = EndOfHeaders;
				continue ;
			}
			if (*in == ':')
			{
				key = std::string(head, in);
				std::transform(key.begin(), key.end(), key.begin(), ::tolower);
				head = in + 1;
				state = RequestHeaderValueStart;
				continue ;
			}
		}

		if (state == RequestHeaderValueStart)
		{
			if (*in == '\r')
				state = RequestHeaderValue;
			continue ;
		}

		if (state == RequestHeaderValue)
		{
			if (*in == '\n')
			{
				value = std::string(head, in - 1);
				head = in + 1;
				state = RequestHeaderKey;
				req.headers[key] = value;
				key.clear();
				value.clear();
				continue ;
			}
			else
				return ParsingError;
		}

		if (state == EndOfHeaders)
		{
			if (*in == '\n')
			{
				skipSpaces(req);
				state = RequestBody;
				head = in;
			}
			else
				return ParsingError;
		}

		if (state == RequestBody)
		{
			body = std::string(head + 1, end);
			return (checkHeaders(req, body, head + 1, end));
		}
	}
	return ParsingIncompleted;
}

void    RequestParser::findQuery(Request& req)
{
	unsigned long i;

	i = req.path.find('?');
	if (i != std::string::npos)
	{
		req.query.assign(req.path, i + 1, req.query.size() - i - 1);
		req.path.erase(i, req.path.size() - i);
	}
}

inline bool RequestParser::checkVersion(std::string &version)
{
	return (version == "1.1" || version == "1.0");
}

void    RequestParser::skipSpaces(Request& req)
{
	std::map<std::string, std::string>::iterator it = req.headers.begin();
	while (it != req.headers.end())
	{
		while (it->second.find(' ') == 0 || it->second.rfind(' ') == it->second.length() - 1)
		{
			if (it->second.find(' ') == 0)
				it->second.erase(0, 1);
			else
				it->second.erase(it->second.length() - 1, 1);
		}
		it++;
	}
}

RequestParser::Result RequestParser::checkHeaders(Request& req, std::string &body, const char *begin_body, const char *end)
{
	if (req.headers.find("transfer-encoding") != req.headers.end() && req.headers.find("transfer-encoding")->second == "chunked") {
		return parsingChunkedBody(req, begin_body, end);
	}
	else if (req.headers.find("content-length") != req.headers.end()) {
		std::istringstream(req.headers.find("content-length")->second) >> content_length;
		if (content_length != 0 && content_length <= body_size_limit) {
			if (req.headers.find("transfer-encoding") == req.headers.end() ||
					req.headers.find("transfer-encoding")->second != "chunked") {
				return parsingBody(req, body);
			} else {
				return ParsingError;
			}
		} else {
			return ParsingError;
		}
	}
	return ParsingCompleted;
}

RequestParser::Result RequestParser::parsingBody(Request& req, std::string &body)
{
	if (body.size() >= content_length) {
		req.body = body.substr(0, content_length);
		return ParsingCompleted;
	}
	return ParsingError;
}

RequestParser::Result RequestParser::parsingChunkedBody(Request& req, const char *begin_body, const char *end)
{
	const char *head = begin_body;
	const char *in = begin_body;
	BodyState state = StartBodyParse;
	int size;

	while (in++ != end)
	{
		if (state == StartBodyParse)
		{
			if (*in == '\r')
				state = ParseSizeOfChunk;
			continue ;
		}

		if (state == ParseSizeOfChunk)
		{
			if (*in == '\n')
			{
				size = strtol(std::string(head, in - 1).c_str(), NULL, 16);
				head = in + 1;
				if (size > 0)
				{
					state = ParseChunk;
					continue ;
				}
				else
					return ParsingCompleted;
			}
			else
				return ParsingError;
		}

		if (state == ParseChunk)
		{
			if (--size == 0)
				state = EndOfChunk;
			continue ;
		}

		if (state == EndOfChunk)
		{
			if (*in == '\r')
			{
				state = AddChunkToBody;
				continue ;
			}
			else
				return ParsingError;
		}

		if (state == AddChunkToBody)
		{
			if (*in == '\n')
			{
				req.body += std::string(head, in - 1);
				head = in + 1;
				state = StartBodyParse;
				continue ;
            }
			else
			return ParsingError;
		}
	}
	return ParsingIncompleted;
}

inline bool RequestParser::isControl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

RequestParser::RequestParser(const unsigned long body_size_limit)
	: content_length(0), body_size_limit(body_size_limit) { }

RequestParser::RequestParser(const RequestParser& ref)
	: content_length(ref.content_length), body_size_limit(ref.body_size_limit) { }

RequestParser::~RequestParser() { }
