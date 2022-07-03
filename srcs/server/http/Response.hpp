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
	const std::string mime_type_css  = "text/css";
	const std::string mime_type_txt  = "text/plain";

	const std::string mime_type_gif  = "image/gif";
	const std::string mime_type_jpeg  = "image/jpeg";
	const std::string mime_type_png  = "image/png";
	const std::string mime_type_webp  = "image/webp";
	const std::string mime_type_ico  = "image/vnd.microsoft.icon";
	const std::string mime_type_svg  = "image/svg+xml";

	const std::string mime_type_bin  = "application/octet-stream";
	const std::string mime_type_json  = "application/json";
	const std::string mime_type_js  = "application/javascript";

	enum ConnectionStatus {
		KeepAlive,
		Close,
	};

	class Response {
		public:
			enum StatusCode {
				OK=200,
				Created=201,
				NoContent=204,
				MovedPermanently=301,
				TemporaryRedirect=307,
				PermanentRedirect=308,
				BadRequest=400,
				NotFound=404,
				MethodNotAllowed=405,
				InternalServerError=500,
			};

			class Header {
				public:
					Header();
					~Header();

					void               set(const std::string& key, const std::string& value);
					void               set_content(const int size, const std::string& type = "");
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
			void reset_body();

			const StatusCode& get_status_code() const;
			bool              is_body() const;
		public:
			Header header;
		private:
			StatusCode        _status;
			ConnectionStatus  _connection_status;
			std::string       _body;
			size_t            _body_size;
			int               _protocol_version;
	};

	extern bool is_redirect_code(Response::StatusCode status_code);
	extern Response::StatusCode int_to_status_code(int status_code);
	extern Response::StatusCode str_to_status_code(const std::string& str_status_code);
	extern const std::string status_code_to_str(Response::StatusCode code);
}; // namespace http

#endif
