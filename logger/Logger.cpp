#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "../config/IConfig.hpp"

using namespace logger;

Logger::Logger() { }

Logger::Logger(const Logger& src) : _fields(src._fields) { }

Logger::Logger(const std::string& fields) : _fields(fields) { }

Logger::~Logger() { }

void Logger::debug(const std::string& msg) const {
	std::cerr << "[DEBUG] " << msg << " " << this->_fields << std::endl;
}

void Logger::info(const std::string& msg) const {
	std::cerr << "[INFO] " << msg << " " << this->_fields << std::endl;
}

void Logger::warn(const std::string& msg) const {
	std::cerr << "[WARN] " << msg << " " << this->_fields << std::endl;
}

void Logger::fatal(const std::string& msg) const {
	std::cerr << "[FATAL] " << msg << " " << this->_fields << std::endl;
}

ILogger& Logger::with_field(const std::string& key, const std::string& value) {
	this->_fields += " " + key + "=" + value;
	return *this;
}

void Logger::set_up(const config::ILoggerConfig& conf) {
	(void)conf;
}
