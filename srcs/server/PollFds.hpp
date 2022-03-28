#ifndef POLL_FDS_HPP
# define POLL_FDS_HPP

#include <vector>
#include <set>
#include <poll.h>

namespace server {
	class PollFds {
		private:
			int _listeners_num;

			std::vector<struct pollfd> _base;

			std::set<int> _listener_event_socks;
			std::set<int> _client_event_socks;
		public:
			PollFds(int term_signal_fd);
			~PollFds();

			struct pollfd* get_array();
			int            get_array_size();

			void           add_listener(int listener);
			void           add_client(int sock);

			bool                 check_term_signal();
			const std::set<int>& check_listeners();
			const std::set<int>& check_clients();

			void erase_client(int sock);
	};
};

#endif
