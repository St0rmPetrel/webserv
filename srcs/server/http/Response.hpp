#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

namespace http {
	class Response {
		public:
			enum StatusCode {
				BadRequest=400,
				NotFound=404,
				MethodNotAllowed=405,
			};
			class Header {
				public:
					void set(const std::string& key, const std::string& value);
			};
		public:
			// serialize return raw response
			const std::string serialize() const;

			void write_header(StatusCode code);
			void write(const char* begin, const char* end);
		public:
			Header header;
	};

	extern Response::StatusCode int_to_status_code(int status_code);
};

#endif
