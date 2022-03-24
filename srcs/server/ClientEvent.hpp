#ifndef CLIENT_EVENT_HPP
# define CLIENT_EVENT_HPP

#include <string>

#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class ClientEvent {
		public:
			const int sock;

			enum ProcessStatus { finish_event, ok };
		private:
			const logger::ILogger& _log;
			const Options&         _opts;
		public:
			ClientEvent(const int sock, const logger::ILogger& log, const Options& opts);
			~ClientEvent();
		public:
			ProcessStatus process();
		private:
			char*       _buf;
			std::string _request_buf;
	};
};

#endif
