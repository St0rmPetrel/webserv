#include "utils.hpp"
#include "../server/http/Response.hpp"

bool utils::is_number(const std::string& str) {
	return (str.find_first_not_of("0123456789") == std::string::npos);
}


const std::string utils::detect_file_mime_type(const std::string& file_path) {
	(void)file_path;
	return http::mime_type_html;
}
