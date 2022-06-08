#ifndef SERVER_MUX_HPP
# define SERVER_MUX_HPP

#include <string>
#include <set>
#include <map>
#include <utility> // pair
#include <vector>
#include <exception>

#include "IHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class ServerMux : public IHandler {
		public:
			struct BusyPathException : public std::exception {
				virtual const char* what() const throw();
			};
			struct HandlerExistException : public std::exception {
				virtual const char* what() const throw();
			};
			class Route : public IHandler {
				friend class ServerMux;
				public:
					struct EmptyHandlerException : public std::exception {
						virtual const char* what() const throw();
					};
					struct ExistHandlerException : public std::exception {
						virtual const char* what() const throw();
					};
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
				private:
					ServerMux&  _mux;
					IHandler*   _handler;
					std::string _path;

					std::set<std::string>                          _allow_methods;
					std::set<std::string>                          _allow_hosts;
					std::set<std::pair<std::string, std::string> > _mandatory_headers;
			};
		public:
			ServerMux(const logger::Logger log);
			ServerMux(const ServerMux& mux);
			~ServerMux();

			void      serve_http(Response& res, const Request& req) const;
			IHandler* clone() const;

			void bad_request(Response& res, const Request& req) const;
			void not_found(Response& res, const Request& req) const;
			void method_not_allowed(Response& res, const Request& req) const;

			Route& new_route();

			void set_bad_request(const IHandler& bad_request_handler);
			void set_not_found(const IHandler& not_found_handler);
			void set_method_not_allowed(const IHandler& method_not_allowed_handler);
		private:
			void _add_route(const std::string& path, const Route* route);
		private:
			logger::Logger _log;

			// routes in order of priority search
			// the longest routes path are at the beginning of vector
			// and searching starts from the longest path to the shortest one
			std::vector<const Route*> _routes;

			const IHandler* _bad_request_handler;
			const IHandler* _not_found_handler;
			const IHandler* _method_not_allowed_handler;
	};
};

#endif
