#ifndef LOGGERMOCK_HPP
#define LOGGERMOCK_HPP

#include <string>

#include "ILogger.hpp"

namespace logger {
	// LoggerMock super easy implementation of ILogger interface
	// for using until real Logger is written
	class LoggerMock : public ILogger {
		private:
			std::string fields;
		public:
			LoggerMock();
			LoggerMock(const std::string& fields);
			LoggerMock(const LoggerMock& src);
			~LoggerMock();

			void debug(const std::string& msg);
			void info(const std::string& msg);
			void warn(const std::string& msg);
			void fatal(const std::string& msg);

			ILogger& with_field(const std::string& key, const std::string& value);
	};
}; /* namespace logger */

#endif
