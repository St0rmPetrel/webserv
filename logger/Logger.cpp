#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "../config/IConfig.hpp"

using namespace logger;

Logger::Logger() : _fields(""), _opt() {
}

Logger::Logger(const Logger& src) : _fields(src._fields), _opt(src._opt) { }

Logger::Logger(const std::string& fields) : _fields(fields), _opt() { }

Logger::~Logger() {

}

void Logger::debug(const std::string& msg) {
	if (_opt.enabled_level <= DEBUG)
		_print_message(DEBUG_MSG, msg, BLUE);
	_fields = "";
}

void Logger::info(const std::string& msg) {
	if (_opt.enabled_level <= INFO)
		_print_message(INFO_MSG, msg, GREEN);
	_fields = "";
}

void Logger::warn(const std::string& msg) {
	if (_opt.enabled_level <= WARN)
		_print_message(WARN_MSG, msg, YELLOW);
	_fields = "";
}

void Logger::fatal(const std::string& msg) {
	if (_opt.enabled_level <= FATAL)
		_print_message(FATAL_MSG, msg, RED);
	_fields = "";
}

ILogger& Logger::with_field(const std::string& key, const std::string& value) {
	this->_fields += " " + key + "=" + value;
	return *this;
}

void Logger::set_up(const Options& opts) {
	_opt = opts;
}

std::string Logger::_generate_time_code(void) const
{
	std::ostringstream _str;
	std::time_t _time;
	std::tm* tm;

	_time = time(NULL);
	tm = std::localtime(&_time);

	// date
	_str << (tm->tm_year + 1900) << "/";
	_str << std::setfill('0') << std::setw(2) << (tm->tm_mon + 1) << "/";
	_str << std::setfill('0') << std::setw(2) << (tm->tm_mday);
	// time
	_str << " " << std::setfill('0') << std::setw(2) << tm->tm_hour << ":";
	_str << std::setfill('0') << std::setw(2) << tm->tm_min << ":";
	_str << std::setfill('0') << std::setw(2) << tm->tm_sec;

	return (_str.str());
}

void Logger::_print_message(const std::string & level, const std::string &msg, const std::string & color)
{
	if (!_opt.bfile_output)
		_opt.file_stream << _generate_time_code() << " " << color;
	_opt.file_stream << level << msg << _fields << std::endl;
	if (!_opt.bfile_output)
		_opt.file_stream << BLANK;
}
