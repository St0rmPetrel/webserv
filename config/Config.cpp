#include <string>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

using namespace config;

Config::Config(const logger::ILogger& log) : _log(log) { }

Config::~Config() { }

void Config::parse(const std::string& filename) {
	_log.debug("parse file: " + filename);
}

const ILoggerConfig& Config::get_logger() const {
	return (this->_log_conf);
}

const IServerConfig& Config::get_server() const {
	return (this->_serv_conf);
}

Config::LoggerConfig::LoggerConfig() : _logfile(""), _level(logger::DEBUG) { }

Config::LoggerConfig::~LoggerConfig() { }

void Config::LoggerConfig::setLogfile(const std::string &logfile)
{
	_logfile = logfile;
}

void Config::LoggerConfig::setLevel(int level)
{
	_level = level;
}

const std::string &Config::LoggerConfig::getLogfile() const
{
	return _logfile;
}

int Config::LoggerConfig::getLevel() const
{
	return _level;
}

Config::ServerConfig::ServerConfig() { }

Config::ServerConfig::~ServerConfig() { }
