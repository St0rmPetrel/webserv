#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../../utils/utils.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>

namespace http {
	enum ConnectionStatus {
		KeepAlive,
		Close,
	};

	enum StatusCode {
		BadRequest=400,
		NotFound=404,
		MethodNotAllowed=405,
	};

	class Response {
		public:
			class Header {
				public:
					Header();

					void set(const std::string& key, const std::string& value);
					void set_date();

					const std::string& get(const std::string& key);
				private:
					std::map<std::string, std::string> headers;
			};
		public:
			Response();
			Response(const Response resp);

			// serialize return raw response
			const std::string serialize() const;

			void write_header(StatusCode code);

			void write(const char* begin, const char* end);
		private:
			void _create_status_line(std::ostringstream& _str) const;
			void _create_header(std::ostringstream& _str) const;
			void _create_body(std::ostringstream& _str) const;
		public:
			Header header;
		private:
			StatusCode       _status;
			ConnectionStatus _connection_status;
			std::string      _body;
			int              _body_size;
			int              _protocol_version;
	};

	extern Response::StatusCode int_to_status_code(int status_code);
}; // namespace http

#endif
