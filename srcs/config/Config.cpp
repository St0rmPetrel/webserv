#include <string>
#include <vector>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

using namespace config;

Config::Config(const logger::ILogger& log) : _log(log) { }

Config::~Config() { }

void Config::parse(const std::string& filename) {
	const std::vector<std::string> tokens = this->_lexing(filename);
	const Module                   global_module = this->_parsing(tokens);

	this->_fill_options(global_module);
}

const ILoggerConfig& Config::get_logger() const {
	return (this->_log_conf);
}

const IServerConfig& Config::get_server() const {
	return (this->_serv_conf);
}

Config::LoggerConfig::LoggerConfig() { }

Config::LoggerConfig::~LoggerConfig() { }

Config::ServerConfig::ServerConfig() { }

Config::ServerConfig::~ServerConfig() { }

const std::vector<std::string> Config::_lexing(const std::string& filename) const {
	_log.debug("start procesing a file: " + filename);
	_log.debug("lexing: read all file, delete comments, process includes, and splitting words into tokens");
	return std::vector<std::string>();
}

const Config::Module Config::_parsing(const std::vector<std::string>& tokens) const {
	(void)tokens;
	_log.debug("start parsing tokens into global module structure");
	_log.debug("splitting directives and modules");
	return Config::Module();
}

void Config::_fill_options(const Config::Module& global_module) {
	(void)global_module;
	_log.debug("start filling configuration staf in to logger and server options");
}

Config::Module::Module() { }

Config::Module::~Module() { }
