#ifndef SIMPLE_HANDLER_HPP
# define SIMPLE_HANDLER_HPP

#include <string>

#include "Response.hpp"
#include "IHandler.hpp"

namespace http {
	struct SimpleHandler : public IHandler {
		std::string root;

		SimpleHandler() { }
		SimpleHandler(const std::string& root) : root(root) { }

		void serve_http(Response& res, const Request& req) const {
			std::string body = "<html>\n"
				" <body>\n"
				"  <h1>Simple Handler</h1>\n"
				"  <p>root = "+root+"</p>\n"
				"  <p>req.path = "+req.path+"</p>\n"
				"  <p>req.method = "+req.method+"</p>\n"
				" </body>\n"
				"</html>\n";
			res.write_header(Response::OK);
			res.write(body, http::mime_type_html);
		}
		IHandler* clone() const {
			return (new SimpleHandler(root));
		}
	};
};

#endif
