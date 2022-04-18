#include <sys/socket.h>
#include <unistd.h>

#include "Server.hpp"
#include "Options.hpp"
#include "../logger/ILogger.hpp"
#include "../utils/utils.hpp"
#include "EventManager.hpp"
#include "Event.hpp"

#include "http/Request.hpp"
#include "http/Response.hpp"
#include "http/RequestParser.hpp"

using namespace server;

Server::Server(const logger::Logger &log, const Options& opts)
	: _log(log), _opts(opts), _event_manager(log) {
	for (std::vector<InetAddr>::const_iterator it = _opts.addrs.begin();
			it != _opts.addrs.end(); it++) {
		this->_event_manager.new_listener(*it);
	}
}

Server::~Server() { }

void Server::listen_and_serve() {
	bool loop = true;

	_log.warn("server: listener and serve: start");
	while (loop) {
		// waiting for events
		const std::set<Event*>& events = _event_manager.accept_events();
		// check and process each event
		for (std::set<Event*>::const_iterator it = events.begin(); it != events.end();
				++it) {
			switch ((*it)->type) {
				case Event::terminate :
					loop = false;
					break;
				case Event::listener : {
					int client = _event_manager.accept_client((*it)->sock);
					_clients_listener[client] = (*it)->sock;
					_clients_request[client] = http::Request();
					break;
				}
				case Event::client : {
					char                         recv_buf[_opts.recv_buffer_size];
					int                          bytes_read;
					// TODO bind it to client
					http::Request&               req = _clients_request[(*it)->sock];
					http::Response               res;
					http::RequestParser          parser;
					http::RequestParser::Result  parsing_result;

					bytes_read = recv((*it)->sock, recv_buf, _opts.recv_buffer_size, 0);
					if (bytes_read < 0) {
						_log.fatal(SSTR("recv socket error client_sock = " << (*it)->sock));
						_event_manager.finish_event(*it);
						continue;
					}
					parsing_result = parser.parse(req, recv_buf, recv_buf + bytes_read);

					const http::VirtualServer& virtual_server = _get_client_virtual_server(
							(*it)->sock, req);
					switch (parsing_result) {
						case http::RequestParser::ParsingCompleted :
							virtual_server.mux.serve_http(res, req);
							break;
						case http::RequestParser::ParsingIncompleted :
							continue;
						case http::RequestParser::ParsingError :
							//virtual_server.bad_request_handler.serve_http(res, req);
							break;
					}
					if (_finish_request((*it)->sock, res) < 0) {
						_log.fatal(SSTR("send socket error client_sock = " << (*it)->sock));
						_event_manager.finish_event(*it);
						continue;
					}
					if (req.close) {
						_clients_listener.erase((*it)->sock);
						_clients_request.erase((*it)->sock);
						_event_manager.finish_event(*it);
					} else {
						_clients_request[(*it)->sock] = http::Request();
					}
					break;
				}
			}
		}
	}
	_log.warn("server: listener and serve: end");
}

const http::VirtualServer& Server::_get_client_virtual_server(int client_sock, http::Request& req) {
	// find right virtual server
	(void)client_sock;
	(void)req;
	return _debug_virtual_server;
}

int Server::_finish_request(int client_sock, http::Response& res) {
	const std::string& raw_res = res.serialize();
	std::size_t bytes_write_total = 0;

	for (int bytes_write = 0; bytes_write_total < raw_res.size();
			bytes_write_total += bytes_write) {
		bytes_write = send(client_sock, &(raw_res[bytes_write_total]),
				raw_res.size() - bytes_write_total, 0);
		if (bytes_write < 0) {
			return bytes_write;
		}
	}
	return (0);
}
