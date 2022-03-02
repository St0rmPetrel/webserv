#ifndef SERVER_HPP
# define SERVER_HPP

#include "IServer.hpp"
#include "Options.hpp"
#include "../logger/Logger.hpp"

namespace server {
	class Server : public IServer {
		private:
			logger::Logger _log;
			Options	_opts;
		public:
			Server(logger::Logger log);
			~Server();

			void serve_http();
			void set_up(const Options& opts);
	};
};

#endif
