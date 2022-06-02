#ifndef UTILS_HPP
# define UTILS_HPP

#include <sstream>
#include <string>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define STDIN 0
#define STDOUT 1
#define STDERR 2

namespace utils {
	extern bool is_number(const std::string& str);
};

#endif
