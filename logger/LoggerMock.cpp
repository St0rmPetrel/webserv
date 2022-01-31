#include <string>
#include <sstream>
#include <iostream>

#include "LoggerMock.hpp"

using namespace logger;

LoggerMock::LoggerMock() { }

LoggerMock::LoggerMock(const LoggerMock& src) : fields(src.fields) { }

LoggerMock::LoggerMock(const std::string& fields) : fields(fields) { }

LoggerMock::~LoggerMock() { }

void LoggerMock::debug(const std::string& msg) {
	std::cerr << "[DEBUG] " << msg << " " << this->fields << std::endl;
}

void LoggerMock::info(const std::string& msg) {
	std::cerr << "[INFO] " << msg << " " << this->fields << std::endl;
}

void LoggerMock::warn(const std::string& msg) {
	std::cerr << "[WARN] " << msg << " " << this->fields << std::endl;
}

void LoggerMock::fatal(const std::string& msg) {
	std::cerr << "[FATAL] " << msg << " " << this->fields << std::endl;
}

ILogger& LoggerMock::with_field(const std::string& key, const std::string& value) {
	this->fields += " " + key + "=" + value;
	return *this;
}
