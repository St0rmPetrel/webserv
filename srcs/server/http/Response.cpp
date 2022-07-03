#include "Response.hpp"
#include "../../utils/utils.hpp"

using namespace http;

Response::Response() : _status(OK), _connection_status(Close),
	_body_size(0), _protocol_version(11) {
}

Response::Response(const Response& resp)
	: _status(resp._status),
	_connection_status(resp._connection_status),
	_body(resp._body),
	_body_size(resp._body_size),
	_protocol_version(resp._protocol_version) {
}

Response::~Response() { }

const std::string Response::serialize() const {
	std::ostringstream sstr;

	sstr << "HTTP/" << _protocol_version / 10 << "." << _protocol_version % 10 << " ";
	sstr << _status << " " << status_code_to_str(_status) << ENDL;

	sstr << header.str();

	if (!_body.empty()) {
		sstr << _body;
	} else {
		sstr << ENDL;
	}

	 return sstr.str();
}

void Response::write_header(StatusCode code) {
	_status = code;
}

void Response::write(const char* begin, const char* end, const std::string& type) {
	for (const char* it = begin; it != end; ++it) {
		++_body_size;
		_body.push_back(*it);
	}
	header.set_content(_body_size, type);
}

void Response::write(const std::string& str, const std::string& type) {
	_body_size += str.size();
	_body.append(str);
	header.set_content(_body_size, type);
}

void Response::reset_body() {
	_body_size = 0;
	_body.clear();
}

const Response::StatusCode& Response::get_status_code() const {
	return _status;
}

bool Response::is_body() const {
	return !_body.empty();
}

Response::Header::Header() { }

Response::Header::~Header() { }

void Response::Header::set(const std::string& key, const std::string& value) {
	this->_headers[key] = value;
}

void Response::Header::set_content(const int length, const std::string& type) {
	std::ostringstream length_sstr;

	length_sstr << length;
	set("Content-Length", length_sstr.str());
	if (!type.empty()) {
		set("Content-Type", type);
	}
}

void Response::Header::set_connection(const ConnectionStatus status) {
	switch (status) {
		case KeepAlive:
			set("Connection", "keep-alive");
			break;
		default:
			set("Connection", "close");
			break;
	}
}

const std::string Response::Header::get(const std::string& key) const {
	const std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it == _headers.end()) {
		return "";
	}
	return it->second;
}

const std::string Response::Header::str() const {
	std::ostringstream sstr;

	for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
				it != _headers.end(); ++it) {
		sstr << it->first << ": " << it->second << ENDL;
	}
	sstr << ENDL;
	return sstr.str();
}

const std::string http::status_code_to_str(Response::StatusCode code) {
	switch (code) {
		case Response::OK:
			return "OK";
		case Response::Created:
			return "Created";
		case Response::NoContent:
			return "No Content";
		case Response::MovedPermanently:
			return "Moved Permanently";
		case Response::PermanentRedirect:
			return "Permanent Redirect";
		case Response::TemporaryRedirect:
			return "Temporary Redirect";
		case Response::BadRequest:
			return "Bad Request";
		case Response::NotFound:
			return "Not Found";
		case Response::MethodNotAllowed:
			return "Method Not Allowed";
		case Response::InternalServerError:
			return "Internal Server Error";
		default:
			return "Not Found";
	}
}

Response::StatusCode http::int_to_status_code(int status_code) {
	return static_cast<Response::StatusCode>(status_code);
}

Response::StatusCode http::str_to_status_code(const std::string& str_status_code) {
	// It can conntent spaces so I use find
	if (str_status_code.find("200 OK") != std::string::npos) {
		return http::Response::OK;
	} else if (str_status_code.find("201 Created") != std::string::npos) {
		return http::Response::Created;
	} else if (str_status_code.find("204 No Content") != std::string::npos) {
		return http::Response::NoContent;
	} else if (str_status_code.find("301 Moved Permanently") != std::string::npos) {
		return http::Response::MovedPermanently;
	} else if (str_status_code.find("307 Temporary Redirect") != std::string::npos) {
		return http::Response::TemporaryRedirect;
	} else if (str_status_code.find("308 Permanent Redirect") != std::string::npos) {
		return http::Response::PermanentRedirect;
	} else if (str_status_code.find("400 Bad Request") != std::string::npos) {
		return http::Response::BadRequest;
	} else if (str_status_code.find("404 Not Found") != std::string::npos) {
		return http::Response::NotFound;
	} else if (str_status_code.find("405 Method Not Allowed") != std::string::npos) {
		return http::Response::MethodNotAllowed;
	} else if (str_status_code.find("500 Internal Server Error") != std::string::npos) {
		return http::Response::InternalServerError;
	} else {
		return http::Response::OK;
	}
}

bool http::is_redirect_code(Response::StatusCode status_code) {
	switch (status_code) {
	case Response::MovedPermanently:
	case Response::PermanentRedirect:
	case Response::TemporaryRedirect:
		return true;
	default:
		return false;
	}
}
