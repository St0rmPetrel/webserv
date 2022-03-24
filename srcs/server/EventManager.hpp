#ifndef EVENT_MANGER_HPP
# define EVENT_MANGER_HPP

#include <map>
#include <vector>
#include <poll.h>

#include "ClientEvent.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class EventManager {
		private:
			const std::vector<int>& _listeners;
			const logger::ILogger&  _log;
			const Options&          _opts;
		public:
			EventManager(const std::vector<int>& listeners,
					const logger::ILogger& log, const Options& opts);
			~EventManager();
		public:
			ClientEvent& accept_event();
			void finish_event(ClientEvent& event);
		private:
			std::vector<struct pollfd> _fds;
			std::map<int, ClientEvent> _events;
	};
};

#endif
