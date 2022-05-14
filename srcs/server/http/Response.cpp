#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const
{
	std::ostringstream _str;

	_create_status_line(_str);
	_create_header(_str);
	_create_body(_str);
//	return "HTTP/1.1 200 OK\r\n"
//		"Content-Length: 12\r\n"
//		"Connection: close\r\n"
//		"Content-Type: text/html\r\n"
//		"\r\n"
//		"Hello world!";
	 return _str.str();
}

void Response::_create_status_line(std::ostringstream& _str) const
{
	_str << "HTTP/" << _protocol_version / 10 << "." << _protocol_version % 10 << " ";
	_str << _status_code << " " << _convert_status_code_to_string() << ENDL;
}

void Response::_create_header(std::ostringstream& _str) const
{
	// general header
	// date
	_str << "Date: ";
	_str << (_date->tm_year + 1900) << "/";
	_str << std::setfill('0') << std::setw(2) << (_date->tm_mon + 1) << "/";
	_str << std::setfill('0') << std::setw(2) << (_date->tm_mday);
	// time
	_str << " " << std::setfill('0') << std::setw(2) << _date->tm_hour << ":";
	_str << std::setfill('0') << std::setw(2) << _date->tm_min << ":";
	_str << std::setfill('0') << std::setw(2) << _date->tm_sec;
	_str << ENDL;

	_str << "Connection: " << _convert_connection_to_string() << ENDL;

	// entity header
//	_str << "Content-Length: " << _length << ENDL;
	_str << "Content-Length: " << "20" << ENDL;

	// useless!! пока что
	_str << "Content-Type: text/html" << ENDL;


}

void Response::_create_body(std::ostringstream& _str) const
{
	_str << ENDL << "Hello from Nastya!!!" << ENDL;
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

std::string Response::_convert_connection_to_string() const {
	switch (_connection) {
		case ::keepAlive:
			return "keep alive";
		case ::close:
		default:
			return "close";
	}
}

void Response::setProtocolVersion(int protocolVersion)
{
	_protocol_version = protocolVersion;
}

void Response::setStatusCode(int statusCode)
{
	_status_code = statusCode;
}

void Response::setConnection(ConnectionStatus connection)
{
	_connection = connection;
}

void Response::setMessage(const std::string &message)
{
	_message = message;
	_length = _message.size();
}

void Response::setLength(size_t length)
{
	_length = length;
}
