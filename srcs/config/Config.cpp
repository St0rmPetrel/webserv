#include <string>
#include <vector>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

using namespace config;

Config::Config(const logger::Logger &log) : _log(log) { }

Config::~Config() { }

void Config::parse(const std::string& filename) {
	const std::vector<std::string> tokens = this->_lexing(filename);
	const Module                   global_module = this->_parsing(tokens);

	this->_fill_options(global_module);
}

const logger::Options& Config::get_logger() const {
	return (this->_log_opts);
}

const server::Options& Config::get_server() const {
	return (this->_serv_opts);
}

const std::vector<std::string> Config::_lexing(const std::string& filename) {
	_log.debug("start procesing a file: " + filename);
	_log.debug("lexing: read all file, delete comments  and splitting words into tokens");
	(void)filename;
	return std::vector<std::string>();
}

const Config::Module Config::_parsing(const std::vector<std::string>& tokens) {
	(void)tokens;

	_log.debug("start parsing tokens into global module structure");
	_log.debug("splitting directives and modules");
	return Config::Module();
}

void Config::_fill_options(const Config::Module& global_module) {
	(void)global_module;
	this->_serv_opts.recv_buffer_size = 1024;
	http::VirtualServer::Options server;
	http::VirtualServer::Options::Location location;

	location.location_match = "/hello";
	location.root = "/www";
	location.error_page[http::Response::NotFound] = "/whoops.html";

	server.port = 8081;
	server.addr = "127.0.0.1";
	server.listener_backlog = 64;
	server.locations.push_back(location);

	this->_serv_opts.servers.push_back(server);
	_log.debug("start filling configuration staf in to logger and server options");
}

const char* Config::LexingErrorException::what () const throw () {
	return "lexing error";
}

const char* Config::ParsingErrorException::what () const throw () {
	return "parsing error";
}
