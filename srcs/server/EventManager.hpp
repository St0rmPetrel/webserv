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
			// return reference to set of active events
			// (blocking)
			const std::set<ClientEvent*>& accept_events();
			// finish_event delete event and close socket according to it
			// also delete socket from _fds
			void                          finish_event(ClientEvent* event);
		private:
			// all clients open connections
			std::map<int, ClientEvent*> _events;
			// Warden for all file descriptors where some action is expected
			PollFds*                    _fds;

			// set of events with socket where some action happened
			// it is clear in start of every accept_events method
			std::set<ClientEvent*> _active_events;
	};
};

#endif
