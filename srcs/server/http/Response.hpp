#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../../utils/utils.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>

namespace http {
	const std::string mime_type_html = "text/html";
	const std::string mime_type_gif  = "image/gif";
	const std::string mime_type_json  = "application/json";
	const std::string mime_type_txt  = "text/plain";

	enum ConnectionStatus {
		KeepAlive,
		Close,
	};

	class Response {
		public:
			enum StatusCode {
				OK=200,
				BadRequest=400,
				NotFound=404,
				MethodNotAllowed=405,
			};

			class Header {
				public:
					Header();
					~Header();

					void               set(const std::string& key, const std::string& value);
					void               set_content(const int size, const std::string& type);
					void               set_connection(const ConnectionStatus status);

					const std::string  get(const std::string& key) const;
					const std::string  str() const;
				private:
					std::map<std::string, std::string> _headers;
			};
		public:
			Response();
			Response(const Response& resp);
			~Response();

			// serialize return raw response
			const std::string serialize() const;

			void write_header(StatusCode code);

			void write(const char* begin, const char* end,
					const std::string& type = mime_type_txt);
			void write(const std::string& str,
					const std::string& type = mime_type_txt);
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
	extern const std::string status_code_to_str(Response::StatusCode code);
}; // namespace http

#endif
