#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

#include "Options.hpp"

namespace logger {
	class ILogger {
		public:
			virtual ~ILogger() { }

			// set_up logger according to cofiguration
			virtual void set_up(const Options& opts) = 0;

			// Prints message to the stderr
			virtual void debug(const std::string& msg) const = 0;
			virtual void info(const std::string& msg) const = 0;
			virtual void warn(const std::string& msg) const = 0;
			virtual void error(const std::string& msg) const = 0;
			virtual void fatal(const std::string& msg) const = 0;
	};
}; /* namespace logger */

#endif
