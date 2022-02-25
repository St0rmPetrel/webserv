#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "ILogger.hpp"
#include "../config/IConfig.hpp"
#include "../config/Config.hpp"

// TODO: transfer defines to common.h
#define BLANK "\033[0m"
#define RED "\033[31m\xf0\x9f\x94\xa5 "
#define GREEN "\033[32m\xf0\x9f\x93\x8c\xef\xb8\x8f "
#define YELLOW "\033[33m\xe2\x9d\x97\xef\xb8\x8f "
#define BLUE "\033[34m\xf0\x9f\x90\x9e "

namespace logger {

	// Logger super easy implementation of ILogger interface
	class Logger : public ILogger {
		private:
			std::string		_fields;
			std::ostream	*_stream;
			std::fstream	_file_stream;
			bool			_bfile_output;
			int				_enabled_level;

		public:
			Logger();
			Logger(const std::string& fields);
			Logger(const Logger& src);
			~Logger();

			void set_up(const config::ILoggerConfig& conf);

			void debug(const std::string& msg) const;
			void info(const std::string& msg) const;
			void warn(const std::string& msg) const;
			void fatal(const std::string& msg) const;

			// TODO make it better
			ILogger& with_field(const std::string& key, const std::string& value);

	private:
			void _print_message(const std::string & level, const std::string & msg, const std::string & color) const;
			std::string _generate_time_code(void) const;
	};
}; /* namespace logger */

#endif
