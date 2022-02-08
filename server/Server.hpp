#ifndef SERVER_HPP
# define SERVER_HPP

#include "IServer.hpp"
#include "../logger/ILogger.hpp"
#include "../config/IConfig.hpp"

namespace server {
	class Server : public IServer {
		private:
			const logger::ILogger& _log;
		public:
			Server(const logger::ILogger& log);
			~Server();

			void serve_http();
			void set_up(const config::IServerConfig& conf);
	};
};

#endif
