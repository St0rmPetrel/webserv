#ifndef LOGGERMOCK_HPP
#define LOGGERMOCK_HPP

#include <string>

#include "ILogger.hpp"

namespace logger {
	// Logger super easy implementation of ILogger interface
	class Logger : public ILogger {
		private:
			std::string fields;
		public:
			Logger();
			Logger(const std::string& fields);
			Logger(const Logger& src);
			~Logger();

			void debug(const std::string& msg);
			void info(const std::string& msg);
			void warn(const std::string& msg);
			void fatal(const std::string& msg);

			// TODO make it better
			ILogger& with_field(const std::string& key, const std::string& value);
	};
}; /* namespace logger */

#endif
