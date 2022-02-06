#ifndef LOGGERMOCK_HPP
#define LOGGERMOCK_HPP

#include <string>

#include "ILogger.hpp"
#include "../config/IConfig.hpp"

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

			void set_up(const config::ILoggerConfig& conf);

			void debug(const std::string& msg) const;
			void info(const std::string& msg) const;
			void warn(const std::string& msg) const;
			void fatal(const std::string& msg) const;

			// TODO make it better
			ILogger& with_field(const std::string& key, const std::string& value);
	};
}; /* namespace logger */

#endif
