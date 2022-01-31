#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

namespace logger {
	class ILogger {
		public:
			virtual ~ILogger() { }

			// Prints message to the stderr
			virtual void debug(const std::string& msg) = 0;
			virtual void info(const std::string& msg) = 0;
			virtual void warn(const std::string& msg) = 0;
			virtual void fatal(const std::string& msg) = 0;

			// Return new logger with predefined output fields
			virtual ILogger& with_field(const std::string& key, const std::string& value) = 0;
	};
}; /* namespace logger */

#endif
