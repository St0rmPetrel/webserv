#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>

#include "IConfig.hpp"
#include "../logger/ILogger.hpp"

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

			struct Directive {
				std::string              name;
				std::vector<std::string> args;
			};
			struct Module {
				Module();
				~Module();
				std::vector<Module>    modules;
				std::vector<Directive> derctives;
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
		private:
			const std::vector<std::string> _lexing(const std::string& filename) const;
			const Module                   _parsing(const std::vector<std::string>& tokens) const;
			void                           _fill_options(const Module& global_module);
	};
};

#endif
