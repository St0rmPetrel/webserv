#include <string>
#include <vector>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

using namespace config;

Config::Config(const logger::Logger &log) : _log(log) { }

Config::~Config() { }

void Config::parse(const std::string& filename) {
	const std::vector<std::string> tokens = this->_lexing(filename);
	const Module                   global_module = this->_parsing(tokens);

	this->_fill_options(global_module);
}

const logger::Options& Config::get_logger() const {
	return (this->_log_opts);
}

const server::Options& Config::get_server() const {
	return (this->_serv_opts);
}

const std::vector<std::string> Config::_lexing(const std::string& filename) {
	_log.debug("start procesing a file: " + filename);
	_log.debug("lexing: read all file, delete comments  and splitting words into tokens");
	Lexer lexer;
	std::vector<std::string>  tokens = lexer.readFile(filename.c_str());
	return tokens;
}

const Config::Module Config::_parsing(const std::vector<std::string>& tokens) {
	(void)tokens;

	_log.debug("start parsing tokens into global module structure");
	_log.debug("splitting directives and modules");
	return Config::Module();
}

void Config::_fill_options(const Config::Module& global_module) {
	(void)global_module;
	_log.debug("start filling configuration staf in to logger and server options");
}

const char* Config::FileNotFoundException::what () const throw () {
	return "Config file was not found";
}

const char* Config::ReadingConfigFileException::what () const throw () {
	return "Error while reading config file";
}

const char* Config::ParsingErrorException::what () const throw () {
	return "parsing error";
}
