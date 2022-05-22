#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../../utils/utils.hpp"
#include "ResponseUtilities.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>

namespace http {

	class Response {
	public:

		Response() : _protocol_version(11), _status_code(http::Ok), _connection(close), _length(0) {
			std::time_t current_time;
			current_time = time(NULL);
			_date = std::localtime(&current_time);
		}

		// serialize return raw response
		const std::string serialize() const;

		// Setters for private members
		void setProtocolVersion(int protocolVersion);
		void setStatusCode(http::StatusCode statusCode);
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
		http::StatusCode _status_code;

		/// general headers
		http::ConnectionStatus _connection;
		std::tm *_date;

		/// response headers

		/// entity headers
		size_t _length;
		// content type
//		http::ContentType _type;
//		http::ContentSubtype _subtype;

		/// message body
		std::string _message;
/*		public:
			class Header {
				public:
					void set(const std::string& key, const std::string& value);
			};

			void write_header(StatusCode code);
			void write(const char* begin, const char* end);
		public:
			Header header;*/
	};

} // namespace http

#endif
