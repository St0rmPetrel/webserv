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

		Response() : _protocol_version(11), _status_code(200), _connection(close), _length(0) {
			std::time_t current_time;
			current_time = time(NULL);
			_date = std::localtime(&current_time);
		}

		// serialize return raw response
		const std::string serialize() const;

		// Setters for private members
		void setProtocolVersion(int protocolVersion);
		void setStatusCode(int statusCode);
		void setConnection(ConnectionStatus connection);
		void setMessage(const std::string &message);
		void setLength(size_t length);

	private:
		void _create_status_line(std::ostringstream& _str) const;

		void _create_header(std::ostringstream& _str) const;

		void _create_body(std::ostringstream& _str) const;

		std::string _convert_status_code_to_string() const;

		std::string _convert_connection_to_string() const;

	private:
		/// status line
		int _protocol_version;
		int _status_code;

		/// general headers
		http::ConnectionStatus _connection;
		std::tm *_date;

		/// response headers

		/// entity headers
		size_t _length;
		// content type
			//	int _type; // ?
			//	int _subtype; // ?

		/// message body
		std::string _message;
	};

} // namespace http

#endif
