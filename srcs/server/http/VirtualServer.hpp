#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include <string>
#include <vector>
#include <set>
#include <map>

#include "ServerMux.hpp"
#include "ReturnHandler.hpp"
#include "ErrorPageHandler.hpp"
#include "FileServerHandler.hpp"
#include "Response.hpp"
#include "../../logger/Logger.hpp"

namespace http {
	class VirtualServer {
		public:
			struct Options {
				struct Location {
					enum Type { FileServer, Return, CGI };

					std::string           location_match;
					Type                  handler_type;
					std::set<std::string> allow_methods;

					ErrorPageHandler::Options  error_page_opts;
					ReturnHandler::Options     return_opts;
					FileServerHandler::Options file_server_opts;
					// TODO legacy
					std::string               root;

					Location() : handler_type(FileServer) { }
				};

				unsigned short int    port; // Номер порта
				std::string           addr; // IP-адрес
				int                   listener_backlog; // Размер очереди запросов
				std::set<std::string> names; // Имена виртуального сервера
				std::vector<Location> locations;

				Options();
			};
		public:
			VirtualServer(const logger::Logger& log, const Options& opts);
			VirtualServer(const VirtualServer& vs);
			~VirtualServer();

			const unsigned short int&             get_port() const;
			const std::string&                    get_addr() const;
			const std::set<std::string>&          get_names() const;
			const std::vector<Options::Location>& get_locations() const;
		private:
			logger::Logger _log;
			Options        _opts;
		public:
			ServerMux mux;
	};
};

#endif
