#ifndef IHANDLER_HPP
# define IHANDLER_HPP

#include "Request.hpp"
#include "Response.hpp"

namespace http {
	class IHandler {
		public:
			virtual ~IHandler() { }

			virtual void      serve_http(Response& res, const Request& req) const = 0;
			virtual IHandler* clone() const = 0;
	};
};

#endif
