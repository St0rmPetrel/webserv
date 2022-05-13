#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const
{
	return "HTTP/1.1 200 OK\r\n"
		"Content-Length: 12\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"Hello world!";
}

void Response::_create_status_line()
{
	_str << "HTTP/" << _protocol_version / 10 << "." << _protocol_version % 10 << " ";
	_str << _status_code << " " << _convert_status_code_to_string() << std::endl;
}

void _create_header()
{

}

void _create_body()
{

}


std::string Response::_convert_status_code_to_string() const {
	// todo: compare code for http 1.1 and other versions
	// todo: add other codes
	switch (_status_code)
	{
		case 200:
			return "OK";
		case 404:
		default:
			return "Not Found";
	}
}
