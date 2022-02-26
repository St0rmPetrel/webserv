#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>

#include "IConfig.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

namespace config {
	class Config : public IConfig {
		private:
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
			server::Options _serv_opts;
			logger::Options _log_opts;

			const logger::ILogger& _log;
		public:
			Config(const logger::ILogger& log);
			~Config();

			void parse(const std::string& filename);

			const logger::Options& get_logger() const;
			const server::Options& get_server() const;
		private:
			const std::vector<std::string> _lexing(const std::string& filename) const;
			const Module                   _parsing(const std::vector<std::string>& tokens) const;
			void                           _fill_options(const Module& global_module);
	};
};

#endif
