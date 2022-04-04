#ifndef SERVER_HPP
# define SERVER_HPP

#include "IServer.hpp"
#include "Options.hpp"
#include "../logger/Logger.hpp"

namespace server {
	class Server : public IServer {
		private:
			const logger::Logger &_log;
			Options	_opts;
		public:
			Server(const logger::Logger &log);
			~Server();

			// serve_http central method of a Server class which is listen and serve
			// http connection
			void serve_http();
			// set_up set options of a Server class and serve_http behavior
			void set_up(const Options& opts);
	};
};

#endif
