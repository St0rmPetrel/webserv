#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "ILogger.hpp"
#include "Options.hpp"


namespace logger {
	class Logger : public ILogger {
		private:
			Options			_opt;

		public:
			Logger();
			Logger(const Options& opts);
			Logger(const Logger& src);
			~Logger();

			void set_up(const Options& opts);

			void debug(const std::string& msg) const;
			void info(const std::string& msg) const;
			void warn(const std::string& msg) const;
			void error(const std::string& msg) const;
			void fatal(const std::string& msg) const;

	private:
			void        _print_message(const Level& level, const std::string& msg,
					const std::string& color) const;
			std::string _generate_time_code(void) const;
	};
}; /* namespace logger */

#endif
