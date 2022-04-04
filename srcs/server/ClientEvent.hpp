#ifndef CLIENT_EVENT_HPP
# define CLIENT_EVENT_HPP

#include <string>

#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class ClientEvent {
		public:
			// client accepted socket from listener
			const int sock;

			// status of processing event
			// if = finish_event, then this event is deleted from EventManager
			enum ProcessStatus { finish_event, ok };
		private:
			const logger::ILogger& _log;
			const Options&         _opts;
		public:
			ClientEvent(const int sock, const logger::ILogger& log, const Options& opts);
			ClientEvent(const ClientEvent& src);
			~ClientEvent();
		public:
			// major method of the class
			// receive process and response on HTTP requests
			ProcessStatus process();
		private:
			char*       _buf;
			std::string _request_buf;
	};
};

#endif
