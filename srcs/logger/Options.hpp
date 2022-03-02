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

		std::string		file_name;
		std::ofstream	file_stream;
		bool			bfile_output;
		Level			enabled_level;

	};
}; /* namespace logger */

#endif
