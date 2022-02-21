#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "../config/IConfig.hpp"

using namespace logger;

Logger::Logger() : _stream(&std::cerr), _enabled_level(DEBUG) {
}

Logger::Logger(const Logger& src) : _fields(src._fields), _stream(src._stream), _enabled_level(src._enabled_level) { }

Logger::Logger(const std::string& fields) : _fields(fields), _stream(&std::cerr), _enabled_level(DEBUG) { }

Logger::~Logger() {
}

void Logger::debug(const std::string& msg) const {
	if (_enabled_level <= DEBUG)
		_print_message(DEBUG_MSG, msg);
}

void Logger::info(const std::string& msg) const {
	if (_enabled_level <= INFO)
		_print_message(INFO_MSG, msg);
}

void Logger::warn(const std::string& msg) const {
	if (_enabled_level <= WARN)
		_print_message(WARN_MSG, msg);
}

void Logger::fatal(const std::string& msg) const {
	if (_enabled_level <= FATAL)
		_print_message(FATAL_MSG, msg);
}

ILogger& Logger::with_field(const std::string& key, const std::string& value) {
	this->_fields += " " + key + "=" + value;
	return *this;
}

void Logger::set_up(const config::ILoggerConfig& conf) {
	(void)conf;

}

std::string Logger::_generate_time_code(void) const
{
	std::ostringstream _str;
	std::time_t _time;
	std::tm* tm;

	_time = time(nullptr);
	tm = std::localtime(&_time);

	// date
	_str << (tm->tm_year + 1900) << "/" << std::setfill('0') << std::setw(2) << (tm->tm_mon + 1) << "/" << (tm->tm_mday);
	// time
	_str << " " << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec;

	return (_str.str());
}

void Logger::_print_message(const std::string & level, const std::string &msg) const
{
	*_stream << _generate_time_code() << level << msg << _fields << std::endl;
}
