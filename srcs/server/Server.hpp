#ifndef SERVER_HPP
# define SERVER_HPP

#include "IServer.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class Server : public IServer {
		private:
			const logger::ILogger& _log;
			Options                _opts;
		public:
			Server(const logger::ILogger& log);
			~Server();

			void serve_http();
			void set_up(const Options& opts);
	};
};

#endif
