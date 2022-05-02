#ifndef SIMPLE_HANDLER_HPP
# define SIMPLE_HANDLER_HPP

#include "IHandler.hpp"

namespace http {
	struct SimpleHanlder : public IHandler {
		void serve_http(Response& res, const Request& req) const {
			(void)res;
			(void)req;
		}
		IHandler* clone() const {
			return (new SimpleHanlder);
		}
	};
};

#endif
