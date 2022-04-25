#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>

namespace http {
	enum ConnectionStatus {
		keepAlive,
		close
	};

	enum ContentType {
		text,
		image,
		audio,
		video,
		// other types
	};

#define CONTENT_TYPE_TO_TEXT(type) #type;

class Response {
public:
  // serialize return raw response
  const std::string serialize() const;

private:
	std::strign _create_status_line() const;
	std::string _create_header() const;
	std::string _create_body() const;
	std::string _convert_status_code_to_string() const;

private:
	/// status line
	int _protocol_version;
	int _status_code;

	/// general headers
	http::ConnectionStatus _connection;
	std::tm*	_date;

	/// response headers

	/// entity headers
	size_t _length;
	// content type
	int _type; // ?
	int _subtype; // ?

	/// message body
};

} // namespace http

#endif
