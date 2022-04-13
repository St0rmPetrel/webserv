#ifndef SERVER_HPP
# define SERVER_HPP

#include "Options.hpp"
#include "EventManager.hpp"
#include "../logger/Logger.hpp"

namespace server {
	class Server {
		private:
			const logger::Logger& _log;
			Options	              _opts;

			EventManager          _event_manager;
		public:
			Server(const logger::Logger &log, const Options& opts);
			~Server();

			// serve_http central method of a Server class which is listen and serve
			// http connection
			void listen_and_serve();
	};
};

#endif
