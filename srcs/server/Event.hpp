#ifndef EVENT_HPP
# define EVENT_HPP

namespace server {
	struct Event {
		enum Type { listener, client, terminate };

		int  sock;
		Type type;
	};
};

#endif
