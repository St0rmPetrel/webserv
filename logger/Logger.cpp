#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "../config/IConfig.hpp"

using namespace logger;

Logger::Logger() : _stream(&std::cerr), _bfile_output(false), _enabled_level(DEBUG) {
}

Logger::Logger(const Logger& src) : _fields(src._fields), _stream(src._stream), _bfile_output(src._bfile_output), _enabled_level(src._enabled_level) { }

Logger::Logger(const std::string& fields) : _fields(fields), _stream(&std::cerr), _bfile_output(false), _enabled_level(DEBUG) { }

Logger::~Logger() {
	if (_bfile_output)
		_file_stream.close();
}

void Logger::debug(const std::string& msg) const {
	if (_enabled_level <= DEBUG)
		_print_message(DEBUG_MSG, msg, BLUE);
}

void Logger::info(const std::string& msg) const {
	if (_enabled_level <= INFO)
		_print_message(INFO_MSG, msg, GREEN);
}

void Logger::warn(const std::string& msg) const {
	if (_enabled_level <= WARN)
		_print_message(WARN_MSG, msg, YELLOW);
}

void Logger::fatal(const std::string& msg) const {
	if (_enabled_level <= FATAL)
		_print_message(FATAL_MSG, msg, RED);
}

ILogger& Logger::with_field(const std::string& key, const std::string& value) {
	this->_fields += " " + key + "=" + value;
	return *this;
}

void Logger::set_up(const config::ILoggerConfig& conf) {
	const std::string & file = conf.getLogfile();
	if (!file.empty())
	{
		_file_stream.open(file, std::fstream::out | std::fstream::app);
		if (_file_stream.is_open())
		{
			_stream = &_file_stream;
			_bfile_output = true;
		}
		else
			this->warn("logfile error");
	}
	_enabled_level = conf.getLevel();
}

std::string Logger::_generate_time_code(void) const
{
	std::ostringstream _str;
	std::time_t _time;
	std::tm* tm;

	_time = time(nullptr);
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

void Logger::_print_message(const std::string & level, const std::string &msg, const std::string & color) const
{
	if (!_bfile_output)
		*_stream << color;
	*_stream << _generate_time_code() << level << msg << _fields << std::endl;
	if (!_bfile_output)
		*_stream << BLANK;
}
