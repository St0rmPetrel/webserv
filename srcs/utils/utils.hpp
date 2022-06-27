#ifndef UTILS_HPP
# define UTILS_HPP

#include <sstream>
#include <string>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define ENDL "\r\n"


#define BLANK "\033[0m"
#define RED "\033[31m\xf0\x9f\x94\xa5"
#define GREEN "\033[32m\xf0\x9f\x93\x8c\xef\xb8\x8f"
#define YELLOW "\033[33m\xe2\x9d\x97\xef\xb8\x8f"
#define BLUE "\033[34m\xf0\x9f\x90\x9e"

namespace utils {
	extern bool              is_number(const std::string& str);
	extern const std::string detect_file_mime_type(const std::string& file_path);
	extern const std::string file_extension(const std::string& file_path);
	extern bool              file_exist(const std::string& path);
};

#endif
