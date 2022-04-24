#include <string>
#include <vector>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

using namespace config;

Config::Config(const logger::Logger &log) : _log(log) {}

Config::~Config() {}

void Config::parse(const std::string &filename) {
	const std::vector<std::string> tokens = this->_lexing(filename);
	this->_parsing(tokens);

	this->_fill_options(_global_module);
}

const logger::Options &Config::get_logger() const {
	return (this->_log_opts);
}

const server::Options &Config::get_server() const {
	return (this->_serv_opts);
}

void Config::_tokenizer(const std::string &line) {
	std::string token;
	size_t i = 0;
	size_t size = line.length();
	char symbol;

	while (i < size) {
		symbol = line[i];
		switch (symbol) {
			case ' ':
			case '\n':
			case '\t':
			case '\0': {
				if (!token.empty()) {
					_tokens.push_back(token);
					token.clear();
				}
				break;
			}
			case '{':
			case '}':
			case ';': {
				if (!token.empty()) {
					_tokens.push_back(token);
					token.clear();
				}
				_tokens.push_back(line.substr(i, 1));
				break;
			}
			default:
				token += line[i];
		}
		i++;
	}
}

const std::string Config::_readFile(const char *filename) {
	std::string line;
	std::string buf;
	std::string::size_type start_comment = 0;
	std::string::size_type end_comment = 0;

	std::ifstream input_file(filename);
	if (!input_file.is_open()) {
		throw config::Config::FileNotFoundException();
	}
	while (std::getline(input_file, buf)) {
		buf += '\n';
		line += buf;
	}
	input_file.close();
	while (start_comment != std::string::npos) {
		start_comment = line.find_first_of('#');
		end_comment = line.find_first_of('\n', start_comment);
		if (start_comment != std::string::npos)
			line = line.erase(start_comment, end_comment - start_comment);
	}
	return (line);
}

const std::vector<std::string> Config::_lexing(const std::string &filename) {
	_log.info("start processing a file: " + filename);
	_tokenizer(_readFile(filename.c_str()));
	_log.debug("lexing: read all file, deleted comments and splitted words into tokens");
	return _tokens;
}

void Config::_parsing(const std::vector<std::string> &tokens) {
	std::vector<std::string>::const_iterator it = tokens.begin();

	_log.debug("parsing: start parsing tokens into global module structure");
	std::vector<std::string> name;
	name.push_back("global");
	_global_module = _collect_module(name, tokens, it, true);
	_log.debug("parsing: all directives and modules are filled");
}

Config::Directive Config::_collect_directive(const std::vector<std::string> &tokens) const {
	Config::Directive dir;

	if (tokens.empty())
		throw Config::ParsingDirException();

	dir.name = tokens[0];
	dir.args.assign(tokens.begin() + 1, tokens.end());
	return dir;
}

Config::Module Config::_collect_module(const std::vector<std::string> &name, const std::vector<std::string> &tokens,
									   std::vector<std::string>::const_iterator &it, bool is_global) const {
	Config::Module local_module;

	if (name.empty())
		throw Config::ParsingModuleException();

	local_module.name = name[0];
	local_module.args.assign(name.begin() + 1, name.end());
	std::vector<std::string> tokens_accumulator;
	for (; it != tokens.end() && (*it != "}" || is_global); ++it) {
		if (*it == ";") {
			local_module.directives.push_back(_collect_directive(tokens_accumulator));
			_log.debug("parsing: filled directive " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		} else if (*it == "{") {
			local_module.modules.push_back(_collect_module(tokens_accumulator, tokens, ++it, false));
			_log.debug("parsing: filled module " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		} else
			tokens_accumulator.push_back(*it);
	}
	if (it == tokens.end() && !is_global)
		throw Config::ParsingBraceException();
	return local_module;
}

void Config::_fill_options(const Config::Module &global_module) {
	(void) global_module;
	_log.debug("start filling configuration staff in to logger and server options");
}

const char *Config::FileNotFoundException::what() const throw() {
	return "Config file was not found";
}

const char *Config::ReadingConfigFileException::what() const throw() {
	return "Error while reading config file";
}

const char *Config::ParsingDirException::what() const throw() {
	return "Unexpected \";\"";
}

const char *Config::ParsingModuleException::what() const throw() {
	return "Unexpected \"{\"";
}

const char *Config::ParsingBraceException::what() const throw() {
	return "Couldn't match \"{\"";
}