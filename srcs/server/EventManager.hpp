#ifndef EVENT_MANGER_HPP
# define EVENT_MANGER_HPP

#include <map>

#include "Event.hpp"
#include "PollFds.hpp"
#include "../logger/ILogger.hpp"

namespace server {
	class EventManager {
		public:
			struct OpenSocketException : public std::exception {
				virtual const char* what() const throw();
			};
			struct FcntlSocketException : public std::exception {
				virtual const char* what() const throw();
			};
			struct BindSocketException : public std::exception {
				virtual const char* what() const throw();
			};
			struct ListenSocketException : public std::exception {
				virtual const char* what() const throw();
			};
			struct ListenerAcceptException : public std::exception {
				virtual const char* what() const throw();
			};
		private:
			const logger::ILogger&  _log;
			// Warden for all file descriptors where some action is expected
			PollFds                 _fds;
			// All clients open connections
			std::map<int, Event*>   _events;
			// Set of events with socket where some action happened
			// it is clear in start of every accept_events method
			std::set<Event*>       _active_events;
		public:
			EventManager(const logger::ILogger& log);
			~EventManager();

			// Return reference to set of active events
			// (blocking)
			const std::set<Event*>& accept_events();

			// finish_event delete event and close socket according to it
			// also delete socket from _fds
			void finish_event(Event* event);
			// new_listener create, binds and set in listener position socket on
			// iaddr
			int  new_listener(const std::string& addr, unsigned short int port, int backlog);
			// accept_client accept client form listener
			int  accept_client(int listener);
		private:
			// return event from _events map if exist or create new and save it in _events
			Event* _get_event(int sock, Event::Type type);
	};
};

#endif
