#ifndef LOGGER_OPTIONS_HPP
# define LOGGER_OPTIONS_HPP

#include <string>

namespace logger {

	enum Level {
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	struct Options {
		Options();
		Options(const std::string& file_name, const Level& level, bool with_color = true);
		Options(const Options& ref);
		~Options();

		Options& operator=(const Options& rh);

		std::string		file_name;
		Level			level;
		bool            with_color;
	};
	extern Level       str_to_level(const std::string& level);
	extern std::string level_to_str(const Level& level);
	extern std::string level_to_msg(const Level& level);
}; /* namespace logger */

#endif
