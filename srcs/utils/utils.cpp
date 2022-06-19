#include <string>

#include "utils.hpp"
#include "../server/http/Response.hpp"

bool utils::is_number(const std::string& str) {
	return (str.find_first_not_of("0123456789") == std::string::npos);
}


const std::string utils::detect_file_mime_type(const std::string& file_path) {
	size_t position = file_path.find_last_of(".");
	if (position == std::string::npos) {
		return http::mime_type_bin;
	}
	std::string extension = file_path.substr(position+1);
	if (extension == "html") {
		return http::mime_type_html;
	} else if (extension == "css") {
		return http::mime_type_css;
	} else if (extension == "txt") {
		return http::mime_type_txt;
	} else if (extension == "gif") {
		return http::mime_type_gif;
	} else if (extension == "jpeg" || extension == "jpg" || extension == "jpe") {
		return http::mime_type_jpeg;
	} else if (extension == "png") {
		return http::mime_type_png;
	} else if (extension == "webp") {
		return http::mime_type_webp;
	} else if (extension == "bin") {
		return http::mime_type_bin;
	} else if (extension == "json") {
		return http::mime_type_json;
	} else if (extension == "js") {
		return http::mime_type_js;
	}
	return http::mime_type_bin;
}
