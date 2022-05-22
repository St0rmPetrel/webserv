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

void Config::_fill_options(const Config::Module& module) {
	for (std::vector<Directive>::const_iterator it = module.directives.begin();
			it != module.directives.end(); ++it) {
		if (module.name == "global") {
			_log.debug("start fillin main module derective " + it->name);
		} else if (module.name == "http") {
			_log.debug("start fillin http module derective " + it->name);
		} else if (module.name == "server") {
			_log.debug("start fillin server module derective " + it->name);
		} else {
			_log.fatal("unknown name " + module.name);
			// throw error
		}
	}
	for (std::vector<Module>::const_iterator it = module.modules.begin();
			it != module.modules.end(); ++it) {
		_log.debug("start filling module " + it->name);
		this->_fill_options(*it);
	}
	return;
//	this->_serv_opts.recv_buffer_size = 1024;
//	http::VirtualServer::Options server;
//	http::VirtualServer::Options::Location location;
//
//	location.location_match = "/hello";
//	location.root = "/www";
//	location.error_page[http::Response::NotFound] = "/whoops.html";
//
//	server.port = 8081;
//	server.addr = "127.0.0.1";
//	server.listener_backlog = 64;
//	server.locations.push_back(location);
//
//	this->_serv_opts.servers.push_back(server);
//	_log.debug("start filling configuration staf in to logger and server options");
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

std::string Config::Print(void) {
    std::ostringstream _str;
    int indent = 0;

    std::vector<Directive>::const_iterator it_dir = _global_module.directives.begin();
    for ( ; it_dir != _global_module.directives.end(); ++it_dir) {
        _str << _print_dir(*it_dir, indent);
    }
    std::vector<Module>::const_iterator it_mod = _global_module.modules.begin();
    for ( ; it_mod != _global_module.modules.end(); ++it_mod) {
        _str << _print_module(*it_mod, indent);
    }

    return (_str.str());
}

std::string Config::_print_dir(const Directive& dir, int indent) {
    std::ostringstream _str;
    std::string strIndent;
    for (int i = 0; i < indent; ++i) {
        strIndent += "    ";
    }

    _str << strIndent << dir.name;
    std::vector<std::string>::const_iterator it_dir_arg = dir.args.begin();
    for ( ; it_dir_arg != dir.args.end(); ++it_dir_arg) {
        _str << " " << *it_dir_arg;
    }
    _str << std::endl;
    return (_str.str());
}

std::string Config::_print_module(const Module& mod, int indent) {
    std::ostringstream _str;
    std::string strIndent;
    for (int i = 0; i < indent; ++i) {
        strIndent += "    ";
    }

    _str << strIndent << mod.name;
    std::vector<std::string>::const_iterator it_mod_arg = mod.args.begin();
    for ( ; it_mod_arg != mod.args.end(); ++it_mod_arg) {
        _str << " " << *it_mod_arg;
    }
    _str << " {" << std::endl;
    std::vector<Directive>::const_iterator it_dir = mod.directives.begin();
    for ( ; it_dir != mod.directives.end(); ++it_dir) {
        _str << _print_dir(*it_dir, indent + 1);
    }
    std::vector<Module>::const_iterator it_mod = mod.modules.begin();
    for ( ; it_mod != mod.modules.end(); ++it_mod) {
        _str << _print_module(*it_mod, indent + 1);
    }
    _str << strIndent << "}" << std::endl;
    return (_str.str());
}
