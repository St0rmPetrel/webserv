#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "Options.hpp"

using namespace logger;

Logger::Logger() : _opt() {
	_file_stream.open("/dev/stderr", std::fstream::out | std::fstream::app);
}

Logger::Logger(const Logger& src) : _opt(src._opt)
{
	_bfile_output = src._bfile_output;
	_file_stream.open(src._opt.file_name, std::fstream::out | std::fstream::app);
	if (!_file_stream.is_open())
	{
		_bfile_output = false;
		_file_stream.open("/dev/stderr", std::fstream::out | std::fstream::app);
		// todo: check if it's open
	}
}

Logger::~Logger() {
	_file_stream.close();
}

void Logger::debug(const std::string& msg) {
	if (_opt.enabled_level <= DEBUG)
		_print_message(DEBUG_MSG, msg, BLUE);
}

void Logger::info(const std::string& msg) {
	if (_opt.enabled_level <= INFO)
		_print_message(INFO_MSG, msg, GREEN);
}

void Logger::warn(const std::string& msg) {
	if (_opt.enabled_level <= WARN)
		_print_message(WARN_MSG, msg, YELLOW);
}

void Logger::fatal(const std::string& msg) {
	if (_opt.enabled_level <= FATAL)
		_print_message(FATAL_MSG, msg, RED);
}

void Logger::set_up(const Options& opts)
{
	this->_opt = opts;
}

std::string Logger::_generate_time_code(void) const
{
	std::ostringstream str;
	std::time_t current_time;
	std::tm* tm;

	current_time = time(NULL);
	tm = std::localtime(&current_time);

	// date
	str << (tm->tm_year + 1900) << "/";
	str << std::setfill('0') << std::setw(2) << (tm->tm_mon + 1) << "/";
	str << std::setfill('0') << std::setw(2) << (tm->tm_mday);
	// time
	str << " " << std::setfill('0') << std::setw(2) << tm->tm_hour << ":";
	str << std::setfill('0') << std::setw(2) << tm->tm_min << ":";
	str << std::setfill('0') << std::setw(2) << tm->tm_sec;

	return (str.str());
}

void Logger::_print_message(const std::string & level, const std::string &msg, const std::string & color)
{
	if (!_bfile_output)
		_file_stream << _generate_time_code() << " " << color;
	_file_stream << level << msg << std::endl;
	if (!_bfile_output)
		_file_stream << BLANK;
}
