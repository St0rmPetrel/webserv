#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>

#include "IConfig.hpp"
#include "../logger/ILogger.hpp"
#include "../logger/Logger.hpp"

namespace config {
	class Config : public IConfig {
		private:
			class LoggerConfig : public ILoggerConfig {
				public:
					LoggerConfig();
					~LoggerConfig();
			};
			class ServerConfig : public IServerConfig {
				public:
					ServerConfig();
					~ServerConfig();
			};
		private:
			ServerConfig _serv_conf;
			LoggerConfig _log_conf;

			logger::Logger _log;
		public:
			Config(logger::Logger log);
			~Config();

			void parse(const std::string& filename);

			const ILoggerConfig& get_logger() const;
			const IServerConfig& get_server() const;
	};
};

#endif
