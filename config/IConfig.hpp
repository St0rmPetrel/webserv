#ifndef ICONFIG_HPP
# define ICONFIG_HPP

#include <string>

namespace config {
	class IConfig {
		public:
			virtual ~IConfig() { }

			// parse file on path in argument and fill config data
			// throw exeption in case of bad config file
			virtual void parse(const std::string& filename) = 0;
	};
}; /* namespace config */

#endif
