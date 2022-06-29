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

					void push_back_handler(const IHandler& handler);
					void mux_register(const std::string& path);

					void      serve_http(Response& res, const Request& req) const;
					IHandler* clone() const;
				private:
					ServerMux&             _mux;
					std::string            _path;
					std::vector<IHandler*> _handler_chain;

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

			void bad_request(Response& res) const;
			void not_found(Response& res) const;
			void method_not_allowed(Response& res) const;

			Route& new_route();
		private:
			void _add_route(const std::string& path, const Route* route);
		private:
			logger::Logger _log;

			// for save memory managment between new_route and handle functions
			IHandler* _new_route;
			// routes in order of priority search
			// the longest routes path are at the beginning of vector
			// and searching starts from the longest path to the shortest one
			std::vector<const Route*> _routes;
	};
};

#endif
