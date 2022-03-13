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

		Level			enabled_level;
		std::string		file_name;

	};
}; /* namespace logger */

#endif
