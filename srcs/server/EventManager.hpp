#ifndef EVENT_MANGER_HPP
# define EVENT_MANGER_HPP

#include <map>
#include <set>
#include <vector>
#include <poll.h>

#include "ClientEvent.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class EventManager {
		private:
			std::set<int>           _listeners;
			const logger::ILogger&  _log;
			const Options&          _opts;
		public:
			EventManager(const logger::ILogger& log, const Options& opts);
			~EventManager();
		public:
			ClientEvent* accept_event();
			void finish_event(ClientEvent* event);
		private:
			std::vector<struct pollfd> _fds;
			std::map<int, ClientEvent> _events;
		private:
			void _erase_from_fds(int sock);
	};
};

#endif
