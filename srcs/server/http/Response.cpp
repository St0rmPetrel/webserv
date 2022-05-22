#include "Response.hpp"

using namespace http;

const std::string Response::serialize() const
{
	std::ostringstream _str;

	_create_status_line(_str);
	_create_header(_str);
	_create_body(_str);
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
	_str << "Content-Length: " << _length << ENDL;

	// useless!! пока что
	_str << "Content-Type: text/html" << ENDL;


}

void Response::_create_body(std::ostringstream& _str) const
{
	_str << ENDL << _message << ENDL;
}


std::string Response::_convert_status_code_to_string() const {
	switch (_status_code)
	{
		case Continue:
			return "Continue";
		case SwitchingProtocols:
			return "Switching Protocols";
		case Ok:
			return "OK";
		case Created:
			return "Created";
		case Accepted:
			return "Accepted";
		case NonAuthoritativeInformation:
			return "Non-Authoritative Information";
		case NoContent:
			return "No Content";
		case ResetContent:
			return "Reset Content";
		case PartialContent:
			return "Partial Content";
		case MultipleChoices:
			return "Multiple Choices";
		case MovedPermanently:
			return "Moved Permanently";
		case Found:
			return "Found";
		case SeeOther:
			return "Not Modified";
		case NotModified:
			return "See Other";
		case UseProxy:
			return "Use Proxy";
		case TemporaryRedirect:
			return "Temporary Redirect";
		case BadRequest:
			return "Bad Request";
		case Unauthorized:
			return "Unauthorized";
		case PaymentRequired:
			return "Payment Required";
		case Forbidden:
			return "Forbidden";
		case NotFound:
			return "Not Found";
		case MethodNotAllowed:
			return "Method Not Allowed";
		case ProxyAuthenticationRequired:
			return "Proxy Authentication Required";
		case RequestTimeOut:
			return "Request Tim-out";
		case Conflict:
			return "Conflict";
		case Gone:
			return "Gone";
		case LengthRequired:
			return "Length Required";
		case PreconditionFailed:
			return "Precondition Failed";
		case RequestEntityTooLarge:
			return "Request Entity Too Large";
		case RequestURITooLarge:
			return "Request URI Too Large";
		case UnsupportedMediaType:
			return "Unsupported Media Type";
		case RequestedRangeNotSatisfiable:
			return "Requested Range Not Satisfiable";
		case ExpectationFailed:
			return "Expectation Failed";
		case InternalServerError:
			return "Internal Server Error";
		case NotImplemented:
			return "Not Implemented";
		case BadGateway:
			return "Bad Gateway";
		case ServiceUnavailable:
			return "Service Unavailable";
		case GatewayTimeOut:
			return "Gateway Tim-out";
		case HTTPVersionNotSupported:
			return "HTTP Version Not Supported";
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

void Response::setStatusCode(http::StatusCode statusCode)
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