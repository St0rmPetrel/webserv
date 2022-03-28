#include "Options.hpp"

logger::Options::Options() : enabled_level(DEBUG), file_name("")
{
}

logger::Options::Options(const logger::Options &ref) {
	*this = ref;
}

logger::Options::~Options() {
}

logger::Options &logger::Options::operator=(const logger::Options &rh) {
	if (this == &rh)
		return (*this);
	enabled_level = rh.enabled_level;
	file_name = rh.file_name;
	return (*this);
}
