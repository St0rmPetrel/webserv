#include "utils.hpp"

bool utils::is_number(const std::string& str) {
	return (str.find_first_not_of("0123456789") == std::string::npos);
}
