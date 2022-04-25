#ifndef POLL_FDS_HPP
# define POLL_FDS_HPP

#include <vector>
#include <set>
#include <poll.h>

namespace server {
	class PollFds {
		public:
			struct PollException : public std::exception {
				virtual const char* what() const throw();
			};
		private:
			// number of listeners socket
			int _listeners_num;

			std::vector<struct pollfd> _base;

			// set of active listeners events
			std::set<int> _listener_event_socks;
			// set of active client events
			std::set<int> _client_event_socks;
			// set of active termination events
			std::set<int> _term_event_socks;
		public:
			// push term_fd to base
			PollFds(int term_fd);
			// close of all sockets(file descriptors)
			~PollFds();

			// do_poll poll syscall wrapper, throw exception in case of error
			void do_poll();
			// add new listener in class
			void add_listener(int listener);
			// add new client in class
			void add_client(int sock);
			// erase sock from class and close it
			void erase_sock(int sock);

			// check actions on termination socket after poll call
			const std::set<int>& check_term();
			// check actions on listeners sockets after poll call
			const std::set<int>& check_listeners();
			// check actions on clients sockets after poll call
			const std::set<int>& check_clients();
	};
};

#endif
