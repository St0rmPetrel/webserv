#ifndef ICONFIG_HPP
# define ICONFIG_HPP

#include <string>

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

namespace config {
	class IConfig {
		public:
			virtual ~IConfig() { }

			// parse file on path in argument and fill config data
			// throw exeption in case of bad config file
			virtual void parse(const std::string& filename) = 0;

			// return logger configuration options
			virtual const logger::Options& get_logger() const = 0;
			// return server configuration options
			virtual const server::Options& get_server() const = 0;
	};
}; /* namespace config */

#endif
