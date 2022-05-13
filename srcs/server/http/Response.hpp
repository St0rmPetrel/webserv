#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>

#define ENDL "\r\n"

namespace http {
	enum ConnectionStatus {
		keepAlive,
		close
	};

	// Есть ли необходимость в этом поле???
	enum ContentType {
		text,
		image,
		audio,
		video,
		// other types
	};

	class Response {
	public:

		Response() : _protocol_version(11), _status_code(200), _connection(close), _length(17) {
		}

		// serialize return raw response
		const std::string serialize() const;

	private:
		void _create_status_line() const;

		void _create_header() const;

		void _create_body() const;

		std::string _convert_status_code_to_string() const;

		std::string _convert_connection_to_string() const;

	private:
		mutable std::ostringstream _str; //todo: проверить стандарт ключевого слова mutable: К СОЖАЛЕНИЮ НЕ 98

		/// status line
		int _protocol_version;
		int _status_code;

		/// general headers
		http::ConnectionStatus _connection;
		mutable std::tm *_date; // explicitly initialized in constructor or when serialize method called

		/// response headers

		/// entity headers
		size_t _length;
		// content type
//	int _type; // ?
//	int _subtype; // ?

		/// message body
	};

} // namespace http

#endif
