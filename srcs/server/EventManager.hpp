#ifndef EVENT_MANGER_HPP
# define EVENT_MANGER_HPP

#include <map>
#include <poll.h>

#include "ClientEvent.hpp"
#include "PollFds.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class EventManager {
		private:
			const logger::ILogger&  _log;
			const Options&          _opts;
		public:
			EventManager(const logger::ILogger& log, const Options& opts);
			~EventManager();
		public:
			const std::set<ClientEvent*>& accept_events();
			void                          finish_event(ClientEvent* event);
		private:
			std::map<int, ClientEvent*> _events;
			PollFds*                    _fds;

			std::set<ClientEvent*> _active_events;
	};
};

#endif
