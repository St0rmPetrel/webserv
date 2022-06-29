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

using namespace server;

Server::Server(const logger::Logger &log, const Options& opts)
	: _log(log), _opts(opts), _event_manager(log) {
	// create and set virtual_servers based on parsed configuration file
	// also creates listeners and field meta data for listeners indexing
	for (std::vector<http::VirtualServer::Options>::const_iterator it = _opts.servers.begin();
			it != _opts.servers.end(); ++it) {
		int listener = 0;

		if ((listener = _find_listener(it->addr, it->port)) < 0) {
			listener = _event_manager.new_listener(it->addr, it->port, it->listener_backlog);
			_listeners_virtual_servers[listener] = std::vector<http::VirtualServer>();
		}
		_listeners_virtual_servers[listener].push_back(http::VirtualServer(_log, *it));
	}
}

Server::~Server() { }

void Server::listen_and_serve() {
	bool loop = true;

	_log.warn("[Server] listener and serve: start");
	while (loop) {
		// waiting for events
		const std::set<Event*>& events = _event_manager.accept_events();
		// check and process each event
		for (std::set<Event*>::const_iterator it = events.begin(); it != events.end();
				++it) {
			switch ((*it)->type) {
				// Event to terminate the program
				case Event::terminate :
					loop = false;
					break;
				// Event to accept new client via listener
				case Event::listener : {
					int client = _event_manager.accept_client((*it)->sock);
					_clients_listener[client] = (*it)->sock;
					_clients_request[client] = http::Request();
					break;
				}
				// Event to process client request
				case Event::client : {
					char                        recv_buf[_opts.recv_buffer_size];
					http::Request&              req = _clients_request[(*it)->sock];
					http::Response              res;

					// read raw data from socket
					int bytes_read = recv((*it)->sock, recv_buf, _opts.recv_buffer_size, 0);
					_log.debug(SSTR("[Server] recv raw body: \"" <<
								recv_buf << "\" bytes_read=" << bytes_read));
					if (bytes_read <= 0) {
						_log.fatal(SSTR("[Server] recv socket error client_sock = " << (*it)->sock));
						_event_manager.finish_event(*it);
						continue;
					}
					// parse raw data to http request representation
					http::RequestParser::Result parsing_result = _request_parser.parse(req,
							recv_buf, recv_buf + bytes_read);

					// choose virtual_server (virtual_server - have specific individual
					// configuration for processing specific request) of a client
					// choice comes from clients listener address and port
					// and name of a host in the request header if exist
					const http::VirtualServer& virtual_server = _get_client_virtual_server(
							(*it)->sock, req);
					// processing request or go to continue reading it
					switch (parsing_result) {
						case http::RequestParser::ParsingCompleted :
							virtual_server.mux.serve_http(res, req);
							break;
						case http::RequestParser::ParsingIncompleted :
							continue;
						case http::RequestParser::ParsingError :
							_log.error("[Server] request parsing error");
							virtual_server.mux.bad_request(res);
							break;
					}
					// send response via socket
					if (_finish_request((*it)->sock, res) < 0) {
						_log.fatal(SSTR("[Server] send socket error client_sock = " << (*it)->sock));
						_event_manager.finish_event(*it);
						continue;
					}
					// if set in request header "Connection: close"
					// or if no in request header "Connection: keep-alive",
					// than close client socket and finish client event in _event_manager
					// else clear client http::Request and start waiting new request
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
	_log.warn("[Server] listener and serve: end");
}

// _get_client_virtual_server choice client virtual_server server based on clients
// listener address:port and host name in request header
const http::VirtualServer& Server::_get_client_virtual_server(int client_sock,
		const http::Request& req) {
	int client_listener = _clients_listener[client_sock];
	const std::vector<http::VirtualServer>& servers = _listeners_virtual_servers[client_listener];
	const http::VirtualServer& default_server = servers[0];
	if (req.host.empty()) {
		return default_server;
	} else {
		for (std::vector<http::VirtualServer>::const_iterator it = servers.begin();
				it != servers.end(); ++it) {
			const std::set<std::string>& server_names = it->get_names();
			if (server_names.find(req.host) != server_names.end()) {
				return (*it);
			}
		}
	}
	return default_server;
}

// _finish_request send raw response via client socket
int Server::_finish_request(int client_sock, const http::Response& res) {
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

// _find_listener find listener socket based on address:port
int Server::_find_listener(const std::string& addr, unsigned short int port) {
	for (std::map<int, std::vector<http::VirtualServer> >::const_iterator it =
			_listeners_virtual_servers.begin(); it != _listeners_virtual_servers.end(); ++it) {
		if (it->second[0].get_addr() == addr && it->second[0].get_port() == port) {
			return (it->first);
		}
	}
	return (-1);
}
