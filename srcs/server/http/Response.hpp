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
	void _create_status_line();
	void _create_header();
	void _create_body();
	std::string _convert_status_code_to_string();

private:
	mutable std::ostringstream _str; //todo: проверить стандарт ключевого слова mutable

	/// status line
	int _protocol_version;
	int _status_code;

	/// general headers
	http::ConnectionStatus _connection;
	std::tm*	_date; // explicitly initialized in constructor or when serialize method called

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
