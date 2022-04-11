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
	this->_parsing(tokens);

	this->_fill_options(_global_module);
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
	(void)filename;
	return std::vector<std::string>();
}

void	Config::_parsing(const std::vector<std::string>& tokens) {
	std::vector<std::string>::const_iterator it = tokens.begin();

	_log.debug("start parsing tokens into global module structure");
	std::vector<std::string> name;
	name.push_back("global");
	_global_module = _collect_module(name, tokens, it, true);
	_log.debug("splitting directives and modules");
}

Config::Directive	Config::_collect_directive(const std::vector<std::string>& tokens) const
{
	Config::Directive dir;

	if (tokens.empty())
		throw Config::ParsingErrorException();

	dir.name = tokens[0];
	dir.args.assign(tokens.begin() + 1, tokens.end());
	return dir;
}

Config::Module	Config::_collect_module(const std::vector<std::string>& name, const std::vector<std::string>& tokens, std::vector<std::string>::const_iterator& it, bool is_global) const
{
	Config::Module local_module;

	if (name.empty())
		throw Config::ParsingErrorException();

	local_module.name = name[0];
	local_module.args.assign(name.begin() + 1, name.end());
	std::vector<std::string> tokens_accumulator;
	for ( ; it != tokens.end() && (*it != "}" || is_global); ++it)
	{
		if (*it == ";")
		{
			local_module.directives.push_back(_collect_directive(tokens_accumulator));
			_log.debug("filled directive " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		}
		else if (*it == "{")
		{
			local_module.modules.push_back(_collect_module(tokens_accumulator, tokens, ++it, false));
			_log.debug("filled module " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		}
		else
			tokens_accumulator.push_back(*it);
	}
	if (it == tokens.end() && !is_global)
		throw Config::ParsingErrorException();
	return local_module;
}

void Config::_fill_options(const Config::Module& global_module) {
	(void)global_module;
	_log.debug("start filling configuration staff in to logger and server options");
}

const char* Config::LexingErrorException::what () const throw () {
	return "lexing error";
}

const char* Config::ParsingErrorException::what () const throw () {
	return "parsing error";
}
