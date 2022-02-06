#include <string>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

using namespace config;

Config::Config(const logger::ILogger& log) : log(log) { }

Config::~Config() { }

void Config::parse(const std::string& filename) {
	log.debug("parse file: " + filename);
}

const ILoggerConfig& Config::get_logger() const {
	return (this->log_conf);
}

const IServerConfig& Config::get_server() const {
	return (this->serv_conf);
}

Config::LoggerConfig::LoggerConfig() { }

Config::LoggerConfig::~LoggerConfig() { }

Config::ServerConfig::ServerConfig() { }

Config::ServerConfig::~ServerConfig() { }
