#include "Options.hpp"

logger::Options::Options() : file_name(""), file_stream("/dev/stderr"), bfile_output(false), enabled_level(DEBUG)
{
}

logger::Options::Options(const logger::Options &ref) {
	*this = ref;
}


logger::Options::~Options() {
	file_stream.close();
}

logger::Options &logger::Options::operator=(const logger::Options &rh) {
	if (this == &rh)
		return (*this);
	file_stream.close();

	file_name = rh.file_name;
	file_stream.open(file_name); // todo: add check
	bfile_output = rh.bfile_output;
	enabled_level = rh.enabled_level;

	return (*this);
}
