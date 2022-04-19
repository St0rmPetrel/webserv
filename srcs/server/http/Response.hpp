#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

namespace http {
	class Response {
		public:
			const std::string serialize() const;
	};
};

#endif
