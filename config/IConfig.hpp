#ifndef ICONFIG_HPP
# define ICONFIG_HPP

#include <string>

namespace config {
	class ILoggerConfig {
		public:
			virtual ~ILoggerConfig() { }

			virtual void setLogfile(const std::string &logfile) = 0;
			virtual void setLevel(int level) = 0;

			virtual const std::string	&getLogfile() const = 0;
			virtual int 				getLevel() const = 0;
	};

	class IServerConfig {
		public:
			virtual ~IServerConfig() { }
	};

	class IConfig {
		public:
			virtual ~IConfig() { }

			// parse file on path in argument and fill config data
			// throw exception in case of bad config file
			virtual void parse(const std::string& filename) = 0;

			// return logger configuration
			virtual const ILoggerConfig& get_logger() const = 0;
			// return server configuration
			virtual const IServerConfig& get_server() const = 0;
	};
}; /* namespace config */

#endif
