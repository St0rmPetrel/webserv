#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const
{
	_create_status_line();
	_create_header();
	_create_body();
//	std::cerr << _str.str();
//	return "HTTP/1.1 200 OK\r\n"
//		"Content-Length: 12\r\n"
//		"Connection: close\r\n"
//		"Content-Type: text/html\r\n"
//		"\r\n"
//		"Hello world!";
	 return _str.str();
}

void Response::_create_status_line() const
{
	_str << "HTTP/" << _protocol_version / 10 << "." << _protocol_version % 10 << " ";
	_str << _status_code << " " << _convert_status_code_to_string() << ENDL;
}

void Response::_create_header() const
{
	// general header

	std::time_t current_time;
	current_time = time(NULL);
	_date = std::localtime(&current_time);
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

	_str << "Content-Length: " << _length << ENDL;

	_str << "Content-Type: text/html" << ENDL;

	_str << "Connection: " << _convert_connection_to_string() << ENDL;

}

void Response::_create_body() const
{
	_str << ENDL << "Hello from Nastya" << ENDL;
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
