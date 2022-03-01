#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

#include "Options.hpp"

namespace logger {

	const std::string DEBUG_MSG = " [DEBUG] ";
	const std::string INFO_MSG = " [INFO] ";
	const std::string WARN_MSG = " [WARN] ";
	const std::string FATAL_MSG = " [FATAL] ";

	class ILogger {
		public:
			virtual ~ILogger() { }

			// set_up logger according to cofiguration
			virtual void set_up(const Options& conf) = 0;

			// Prints message to the stderr
			virtual void debug(const std::string& msg) = 0;
			virtual void info(const std::string& msg) = 0;
			virtual void warn(const std::string& msg) = 0;
			virtual void fatal(const std::string& msg) = 0;

			// Return new logger with predefined output fields
			// TODO make it better
			virtual ILogger& with_field(const std::string& key, const std::string& value) = 0;
	};
}; /* namespace logger */

#endif
