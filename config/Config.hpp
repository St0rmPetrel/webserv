#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>

#include "IConfig.hpp"
#include "../logger/ILogger.hpp"

namespace config {
	class Config : public IConfig {
		private:
			class LoggerConfig : public ILoggerConfig {
				public:
					LoggerConfig();
					~LoggerConfig();

					void setLogfile(const std::string &logfile);
					void setLevel(logger::Level level);

					const std::string	&getLogfile() const;
				logger::Level 				getLevel() const;

				private:
					std::string	_logfile;
				logger::Level 		_level;

			};
			class ServerConfig : public IServerConfig {
				public:
					ServerConfig();
					~ServerConfig();
			};
		private:
			ServerConfig _serv_conf;
			LoggerConfig _log_conf;

			const logger::ILogger& _log;
		public:
			Config(const logger::ILogger& log);
			~Config();

			void parse(const std::string& filename);

			const ILoggerConfig& get_logger() const;
			const IServerConfig& get_server() const;
	};
};

#endif
