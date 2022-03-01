#ifndef LOGGER_OPTIONS_HPP
# define LOGGER_OPTIONS_HPP

#include <fstream>

namespace logger {

	enum Level {
		DEBUG,
		INFO,
		WARN,
		FATAL
	};

	struct Options {
		Options();
		Options(const Options& ref);
		~Options();

		Options& operator=(const Options& rh);

//		std::ostream	*_stream;
// todo: add file name
		std::ofstream	_file_stream;
		bool			_bfile_output;
		Level			_enabled_level;

	};
}; /* namespace logger */

#endif
