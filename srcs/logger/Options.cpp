#include "Options.hpp"

using namespace logger;

Options::Options()
	: file_name("/dev/stderr"), level(WARN), with_color(true) { }

Options::Options(const std::string& file_name, const Level& level, bool with_color)
	: file_name(file_name), level(level), with_color(with_color) { }

Options::Options(const logger::Options &ref) {
	*this = ref;
}

Options::~Options() { }

Options &logger::Options::operator=(const logger::Options &rh) {
	if (this == &rh)
		return (*this);
	level = rh.level;
	file_name = rh.file_name;
	with_color = rh.with_color;
	return (*this);
}

Level logger::str_to_level(const std::string& level) {
	if (level == "debug") {
		return DEBUG;
	} else if (level == "info") {
		return INFO;
	} else if (level == "warn") {
		return WARN;
	} else if (level == "error") {
		return ERROR;
	} else if (level == "fatal") {
		return FATAL;
	}
	return FATAL;
}

std::string logger::level_to_str(const Level& level) {
	switch (level) {
		case DEBUG:
			return "debug";
		case INFO:
			return "info";
		case WARN:
			return "warn";
		case ERROR:
			return "error";
		case FATAL:
			return "fatal";
	}
}

std::string logger::level_to_msg(const Level& level) {
	switch (level) {
		case DEBUG:
			return " [DEBUG] ";
		case INFO:
			return " [INFO] ";
		case WARN:
			return " [WARN] ";
		case ERROR:
			return " [ERROR] ";
		case FATAL:
			return " [FATAL] ";
	}
}
