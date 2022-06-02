#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "ILogger.hpp"
#include "Options.hpp"


const std::string BLANK =  "\033[0m";
const std::string RED = "\033[31m\xf0\x9f\x94\xa5";
const std::string GREEN = "\033[32m\xf0\x9f\x93\x8c\xef\xb8\x8f";
const std::string YELLOW = "\033[33m\xe2\x9d\x97\xef\xb8\x8f";
const std::string BLUE = "\033[34m\xf0\x9f\x90\x9e";

namespace logger {

	// Logger super easy implementation of ILogger interface
	class Logger : public ILogger {
		private:
			std::ostream	*_file_stream;
			std::ofstream	_base_stream;
			bool			_bfile_output;
			Options			_opt;

		public:
			Logger();
			Logger(const Logger& src);
			~Logger();

			void set_up(const Options& opts);

			void debug(const std::string& msg) const;
			void info(const std::string& msg) const;
			void warn(const std::string& msg) const;
			void fatal(const std::string& msg) const;

	private:
			void _print_message(const std::string & level, const std::string & msg, const std::string & color) const;
			std::string _generate_time_code(void) const;
	};

	extern Level string_to_level(const std::string& level);
	extern std::string level_to_string(const Level& level);
}; /* namespace logger */

#endif
