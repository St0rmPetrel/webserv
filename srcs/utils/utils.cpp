#include <string>
#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>
#include <cctype>
#include <iomanip>
#include <sstream>

#include "utils.hpp"
#include "../server/http/Response.hpp"

bool utils::is_number(const std::string& str) {
	return (str.find_first_not_of("0123456789") == std::string::npos);
}


const std::string utils::file_extension(const std::string& file_path) {
	size_t position = file_path.find_last_of(".");
	if (position == std::string::npos) {
		return "";
	}
	return file_path.substr(position+1);
}

const std::string utils::detect_file_mime_type(const std::string& file_path) {
	std::string extension = file_extension(file_path);
	if (extension == "") {
		return http::mime_type_txt;
	} else if (extension == "html") {
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
	} else if (extension == "ico") {
		return http::mime_type_ico;
	} else if (extension == "svg") {
		return http::mime_type_svg;
	} else if (extension == "mp3") {
		return http::mime_type_mp3;
	} else if (extension == "mp4") {
		return http::mime_type_mp4;
	} else if (extension == "mpeg") {
		return http::mime_type_mpeg;
	} else if (extension == "bin") {
		return http::mime_type_bin;
	} else if (extension == "json") {
		return http::mime_type_json;
	} else if (extension == "js") {
		return http::mime_type_js;
	} else if (extension == "pdf") {
		return http::mime_type_pdf;
	} else if (extension == "php") {
		return http::mime_type_php;
	} else if (extension == "eot") {
		return http::mime_type_eot;
	} else if (extension == "woff") {
		return http::mime_type_woff;
	} else if (extension == "woff2") {
		return http::mime_type_woff2;
	} else if (extension == "ttf") {
		return http::mime_type_ttf;
	}
	return http::mime_type_bin;
}

bool utils::file_exist(const std::string& path) {
	std::ifstream f;
	f.open(path.c_str());
    return f.good();
}

const std::string utils::read_file_fd(int fd) {
	char        buffer[1024] = {0};
	std::string ret;

	while (read(fd, buffer, 1023) > 0) {
		ret += std::string(buffer);
	}
	return ret;
}

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

const std::string utils::str_to_lower(const std::string& str) {
	std::string data(str);

	std::transform(data.begin(), data.end(), data.begin(), asciitolower);

	return data;
}

const std::string utils::url_decode(const std::string& SRC) {
	std::string ret;
	char ch;
	int ii;
	for (size_t i=0; i<SRC.length(); i++) {
		if (int(SRC[i])==37) {
			sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
			ch=static_cast<char>(ii);
			ret+=ch;
			i=i+2;
		} else {
			ret+=SRC[i];
		}
	}
	return (ret);
}

const std::string utils::url_encode(const std::string &value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);
		
		// Keep alphanumeric and other accepted characters intact
		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}
		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char) c);
		escaped << std::nouppercase;
	}
	return escaped.str();
}
