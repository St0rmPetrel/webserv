#include <string>
#include <vector>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"

using namespace config;

Config::Config(const logger::ILogger& log) : _log(log) { }

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

const std::vector<std::string> Config::_lexing(const std::string& filename) const {
	_log.debug("start procesing a file: " + filename);
	_log.debug("lexing: read all file, delete comments  and splitting words into tokens");
	return std::vector<std::string>();
}

void	Config::_choice(Config::Module& mod, const std::vector<std::string>& tokens, std::vector<std::string>::const_iterator& it, std::vector<std::string>& tmp) const
{
	if (*it == ";")
	{
		mod.directives.push_back(_collect_directive(tmp));
		_log.debug("filled directive " + tmp[0]);
		tmp.clear();
	}
	else if (*it == "{")
	{
		mod.modules.push_back(_collect_module(tmp, tokens, ++it));
		_log.debug("filled module " + tmp[0]);
		tmp.clear();
	}
	else
		tmp.push_back(*it);
}

Config::Module	Config::_parsing(const std::vector<std::string>& tokens) const {
	Config::Module mod;
	std::vector<std::string> tmp;
	std::vector<std::string>::const_iterator it;


	_log.debug("start parsing tokens into global module structure");
	mod.name = "global";
	for (it = tokens.begin(); it != tokens.end(); ++it)
		_choice(mod, tokens, it, tmp);
	_log.debug("splitting directives and modules");
	return mod;
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

Config::Module	Config::_collect_module(const std::vector<std::string>& name, const std::vector<std::string>& tokens, std::vector<std::string>::const_iterator& it) const
{
	Config::Module mod;

	if (name.empty())
		throw Config::ParsingErrorException();

	mod.name = name[0];
	mod.args.assign(name.begin() + 1, name.end());
	std::vector<std::string> tmp;
	for ( ; it != tokens.end() && *it != "}"; ++it)
		_choice(mod, tokens, it, tmp);
	if (it == tokens.end())
		throw Config::ParsingErrorException();
	return mod;
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
