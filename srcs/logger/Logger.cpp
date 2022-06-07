#include <string>
#include <sstream>
#include <iostream>

#include "Logger.hpp"
#include "../utils/utils.hpp"
#include "Options.hpp"

using namespace logger;

Logger::Logger() : _opt("/dev/stderr", WARN, true) { }

Logger::Logger(const Options& opts) : _opt(opts) { }

Logger::Logger(const Logger& src) : _opt(src._opt) { }

Logger::~Logger() { }

void Logger::debug(const std::string& msg) const {
	if (_opt.level <= DEBUG) {
		_print_message(DEBUG, msg, BLUE);
	}
}

void Logger::info(const std::string& msg) const {
	if (_opt.level <= INFO) {
		_print_message(INFO, msg, GREEN);
	}
}

void Logger::warn(const std::string& msg) const {
	if (_opt.level <= WARN) {
		_print_message(WARN, msg, YELLOW);
	}
}

void Logger::error(const std::string& msg) const {
	if (_opt.level <= ERROR) {
		_print_message(ERROR, msg, RED);
	}
}

void Logger::fatal(const std::string& msg) const {
	if (_opt.level <= FATAL) {
		_print_message(FATAL, msg, RED);
	}
}

void Logger::set_up(const Options& opts) {
	this->_opt = opts;
}

std::string Logger::_generate_time_code(void) const {
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

void Logger::_print_message(const Level& level, const std::string& msg,
		const std::string& color) const {
	std::ofstream output;

	output.open(_opt.file_name.c_str(), std::fstream::out | std::fstream::app);

	output << _generate_time_code() << " ";
	if (_opt.with_color) {
		output << color;
	}
	output << level_to_msg(level) << msg;
	if (_opt.with_color) {
		output << BLANK;
	}
	output << std::endl;

	output.close();
}
