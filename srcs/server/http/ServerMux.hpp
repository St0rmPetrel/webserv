#ifndef SERVER_MUX_HPP
# define SERVER_MUX_HPP

#include <string>
#include <set>
#include <map>
#include <utility> // pair
#include <vector>

#include "IHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace http {
	class ServerMux : public IHandler {
		public:
			class Route : public IHandler {
				friend class ServerMux;
				private:
					ServerMux& _mux;
					IHandler*  _handler;

					std::set<std::string>                          _allow_methods;
					std::set<std::string>                          _allow_hosts;
					std::set<std::pair<std::string, std::string> > _mandatory_headers;
				private:
					Route(ServerMux& mux);
					Route(const Route& r);
				public:
					~Route();

					Route& method(const std::string& method);
					Route& host(const std::string& host);
					Route& header(const std::pair<std::string, std::string>& pair);

					bool match(const Request& req) const;

					// handle registers the handler for the given pattern.
					// If a handler already exists for pattern, handle throw exception.
					void handle(const std::string& path, const IHandler& handler);

					void      serve_http(Response& res, const Request& req) const;
					IHandler* clone() const;
			};
		public:
			ServerMux();
			ServerMux(const ServerMux& mux);
			~ServerMux();

			void      serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;

			void bad_request(Response& res, const Request& req) const;
			void not_found(Response& res, const Request& req) const;
			void method_not_allowed(Response& res, const Request& req) const;

			Route* new_route();

			void add_middleware(const IHandler& handler);
			void set_bad_request(const IHandler& bad_request_handler);
			void set_not_found(const IHandler& not_found_handler);
			void set_method_not_allowed(const IHandler& method_not_allowed_handler);
		private:
			// vector of middlewares to be called after a match is found
			std::vector<IHandler*> _middlewares;

			std::map<std::string, Route*> _routes;

			IHandler* _bad_request_handler;
			IHandler* _not_found_handler;
			IHandler* _method_not_allowed_handler;
	};
};

#endif
