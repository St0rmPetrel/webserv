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

	//if (!_body.empty()) {
	//	std::ostringstream body_size_sstr;
	//	body_size_sstr << _body_size;
	//	header.set("Content-Length", body_size_sstr.str());

	//	if (header.get("Content-Type").empty()) {
	//		header.set("Content-Type", "plain/text");
	//	}
	//}

	sstr << "HTTP/" << _protocol_version / 10 << "." << _protocol_version % 10 << " ";
	sstr << _status << " " << status_code_to_str(_status) << ENDL;

	sstr << header.str();

	sstr << _body << ENDL;

	 return sstr.str();
}

void Response::write_header(StatusCode code) {
	_status = code;
}

void Response::write(const char* begin, const char* end) {
	for (const char* it = begin; it != end; ++it) {
		++_body_size;
		_body.push_back(*it);
	}
}

void Response::write(const std::string& str) {
	_body_size += str.size();
	_body.append(str);
}

Response::Header::Header() { }

Response::Header::~Header() { }

void Response::Header::set(const std::string& key, const std::string& value) {
	this->_headers[key] = value;
}

//const std::string& Response::Header::get(const std::string& key) const {
//	return this->_headers[key];
//}

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
		case Response::BadRequest:
			return "Bad Request";
		case Response::NotFound:
			return "Not Found";
		case Response::MethodNotAllowed:
			return "Method Not Allowed";
		default:
			return "Not Found";
	}
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
