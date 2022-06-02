#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const {
	std::ostringstream sstr;

	if (!_body.empty()) {
		heade.set("Content-Length", _body_size_to_string());

		if (header.get("Content-Type").empty()) {
			heade.set("Content-Type", "plain/text");
		}
	}

	_create_status_line(sstr);
	_create_header(sstr);
	_create_body(sstr);

	 return sstr.str();
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

void Response::write_header(Response::StatusCode code) {
	(void)code;
}

void Response::write(const char* begin, const char* end) {
	(void)begin;
	(void)end;
}

void Response::Header::set(const std::string& key, const std::string& value) {
	(void)key;
	(void)value;
}

Response::StatusCode http::int_to_status_code(int status_code) {
	switch (status_code) {
		case 400:
			return Response::BadRequest;
		case 404:
			return Response::NotFound;
		case 405:
			return Response::MethodNotAllowed;
		default:
			// TODO make exception
			throw "";
	}
}
