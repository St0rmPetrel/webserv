#include "Options.hpp"

logger::Options::Options() : _file_stream("/dev/stderr"), _bfile_output(false), _enabled_level(DEBUG)
{
}

logger::Options::Options(const logger::Options &ref) {
	*this = ref;
}


logger::Options::~Options() {
	_file_stream.close();
}

logger::Options &logger::Options::operator=(const logger::Options &rh) {
	if (this == &rh)
		return (*this);

	_file_stream.close();

//	_file_stream(rh._file_stream); // отсутствует конструктор копирование
	_bfile_output = rh._bfile_output;
	_enabled_level = rh._enabled_level;

	return (*this);
}
