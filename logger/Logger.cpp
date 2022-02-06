#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"

using namespace logger;

Logger::Logger() { }

Logger::Logger(const Logger& src) : fields(src.fields) { }

Logger::Logger(const std::string& fields) : fields(fields) { }

Logger::~Logger() { }

void Logger::debug(const std::string& msg) {
	std::cerr << "[DEBUG] " << msg << " " << this->fields << std::endl;
}

void Logger::info(const std::string& msg) {
	std::cerr << "[INFO] " << msg << " " << this->fields << std::endl;
}

void Logger::warn(const std::string& msg) {
	std::cerr << "[WARN] " << msg << " " << this->fields << std::endl;
}

void Logger::fatal(const std::string& msg) {
	std::cerr << "[FATAL] " << msg << " " << this->fields << std::endl;
}

ILogger& Logger::with_field(const std::string& key, const std::string& value) {
	this->fields += " " + key + "=" + value;
	return *this;
}
