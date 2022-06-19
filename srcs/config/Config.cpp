#include <string>
#include <vector>
#include <sstream>

#include "Config.hpp"
#include "../logger/ILogger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"
#include "../utils/utils.hpp"

#include <exception>

using namespace config;

Config::Config(const logger::Logger &log) : _log(log) {}

Config::~Config() {}

void Config::parse(const std::string &filename) {
	const std::vector<std::string> tokens = this->_lexing(filename);
	this->_parsing(tokens);
	this->_fill_options();
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
	_log.info("[Config] [Lexing] start processing a file: " + filename);
	_tokenizer(_readFile(filename.c_str()));
	_log.debug("[Config] [Lexing] read all file, deleted comments and splitted words into tokens");
	return _tokens;
}

void Config::_parsing(const std::vector<std::string> &tokens) {
	std::vector<std::string>::const_iterator it = tokens.begin();

	_log.debug("[Config] [Parsing] start parsing tokens into global module structure");
	std::vector<std::string> name;
	name.push_back("global");
	_global_module = _collect_module(name, tokens, it, true);
	_log.debug("[Config] [Parsing] all directives and modules are filled");
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
			_log.debug("[Config] [Parsing] filled directive " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		} else if (*it == "{") {
			local_module.modules.push_back(_collect_module(tokens_accumulator, tokens, ++it, false));
			_log.debug("[Config] [Parsing] filled module " + tokens_accumulator[0]);
			tokens_accumulator.clear();
		} else
			tokens_accumulator.push_back(*it);
	}
	if (it == tokens.end() && !is_global)
		throw Config::ParsingBraceException();
	return local_module;
}

void Config::_fill_options() {
	for (std::vector<Directive>::const_iterator it = _global_module.directives.begin();
			it != _global_module.directives.end(); ++it) {
		if (it->name == "error_log") {
			_fill_error_log_directive(*it);
		} else if (it->name == "daemon") {
			// fill daemon option
		} else {
			_log.debug(SSTR("[Config] [Filling] unknown directive name: " << it->name));
			throw FillingUnknownDirectiveException();
		}
	}

	for (std::vector<Module>::const_iterator it = _global_module.modules.begin();
			it != _global_module.modules.end(); ++it) {
		if (it->name == "http") {
			_fill_http_options(*it);
			_log.debug("[Config] [Filling] filled http module");
		} else if (it->name == "events") {
			// for nginx compatibility
			continue;
		} else {
			_log.fatal(SSTR("[Config] [Filling] unknown module name: " << it->name));
			throw FillingUnknownModuleException();
		}
	}
	_log.debug("[Config] [Filling] filled global module");
}

void Config::_fill_http_options(const Module& http_module) {
	http::VirtualServer::Options default_virtual_server_opts;

	_fill_virtual_server_options(default_virtual_server_opts, http_module);

	for (std::vector<Config::Module>::const_iterator it = http_module.modules.begin();
			it != http_module.modules.end(); ++it) {
		if (it->name == "server") {
			http::VirtualServer::Options virtual_server_opts(default_virtual_server_opts);

			_fill_virtual_server_options(virtual_server_opts, *it);
			_serv_opts.servers.push_back(virtual_server_opts);
			_log.debug("[Config] [Filling] filled server module");
		}
	}
}

void Config::_fill_virtual_server_options(http::VirtualServer::Options& virtual_server_opts,
		const Config::Module& server_module) {
	http::VirtualServer::Options::Location default_virtual_server_location_opts;

	for (std::vector<Directive>::const_iterator it = server_module.directives.begin();
			it != server_module.directives.end(); ++it) {
		if (it->name == "listen") {
			_fill_listen_directive(virtual_server_opts, *it);
		} else if (it->name == "server-name") {
			_fill_server_name_directive(virtual_server_opts, *it);
		} else {
			_fill_virtual_server_location_options(default_virtual_server_location_opts,
					server_module);
		}
	}
	for (std::vector<Module>::const_iterator it = server_module.modules.begin();
			it != server_module.modules.end(); ++it) {
		if (it->name == "server" && server_module.name == "http") {
			continue;
		} else if (it->name == "location") {
			http::VirtualServer::Options::Location virtual_server_location_opts(
					default_virtual_server_location_opts);
			_fill_virtual_server_location_options(virtual_server_location_opts, *it);
			virtual_server_opts.locations.push_back(virtual_server_location_opts);
			_log.debug("[Config] [Filling] filled location module");
		} else {
			_log.fatal(SSTR("[Config] [Filling] unknown module name: " << it->name));
			throw FillingUnknownModuleException();
		}
	}
}

void Config::_fill_virtual_server_location_options(
		http::VirtualServer::Options::Location& virtual_server_location_opts,
		const Config::Module& location_module) {
	if (location_module.args.empty() && location_module.name != "server") {
		throw FillingEmptyModuleArgsException();
	}
	// fill location match
	{
		if (location_module.name == "location") {
			virtual_server_location_opts.location_match = location_module.args.at(0);
		} else {
			virtual_server_location_opts.location_match = "/";
		}
	}
	_log.debug(SSTR("[Config] [Filling] fill location match: " <<
				virtual_server_location_opts.location_match));

	for (std::vector<Directive>::const_iterator it = location_module.directives.begin();
			it != location_module.directives.end(); ++it) {
		if (it->name == "root") {
			_fill_root_directive(virtual_server_location_opts, *it);
		} else if (it->name == "error_page") {
			_fill_error_page_directive(virtual_server_location_opts, *it);
		} else if (it->name == "allow_methods") {
			_fill_allow_methods_directive(virtual_server_location_opts, *it);
		} else if (it->name == "index") {
			// fill index staff
		} else if (it->name == "autoindex") {
			// fill index staff
		} else if (it->name == "return") {
			_fill_return_directive(virtual_server_location_opts, *it);
		} else if ((it->name == "listen") && (location_module.name == "server")) {
			continue;
		} else {
			_log.fatal(SSTR("[Config] [Filling] unknown directive name: " << it->name));
			throw FillingUnknownDirectiveException();
		}
	}
}

void Config::_fill_server_name_directive(
		http::VirtualServer::Options& virtual_server_opts,
		const Config::Directive& server_name_dir) {
	if (server_name_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [Filling] empty directive args: " << server_name_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	// fill server_names
	for (std::vector<std::string>::const_iterator it = server_name_dir.args.begin();
			it != server_name_dir.args.end(); ++it) {
		virtual_server_opts.names.insert(*it);
		_log.debug(SSTR("[Config] [Filling] add server_name: name=" << *it));
	}
}

void Config::_fill_error_page_directive(
		http::VirtualServer::Options::Location& location_opts,
		const Config::Directive& error_page_dir) {
	if (error_page_dir.args.size() != 2) {
		_log.fatal(SSTR("[Config] [filling] empty directive args: " << error_page_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}

	const std::string& status_code_str = error_page_dir.args.at(0);
	if (!utils::is_number(status_code_str)) {
		_log.fatal(SSTR("[Config] [Filling] bad directive args: " << status_code_str));
		throw FillingBadDirectiveArgsException();
	}
	int status_code = 0;
	std::istringstream(status_code_str) >> status_code;

	location_opts.error_page_opts.pages_path[http::int_to_status_code(status_code)] =
		error_page_dir.args.at(1);
	_log.debug(SSTR("[Config] [Filling] fill error_page: status code=" << status_code <<
				" error_file_path=" << error_page_dir.args.at(1)));
}

void Config::_fill_allow_methods_directive(
		http::VirtualServer::Options::Location& location_opts,
		const Config::Directive& allow_methods_dir) {
	if (allow_methods_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [filling] empty directive args: " << allow_methods_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	for (std::vector<std::string>::const_iterator it = allow_methods_dir.args.begin();
			it != allow_methods_dir.args.end(); ++it) {
		location_opts.allow_methods.insert(*it);
	}
	_log.debug("[Config] [Filling] fill allow_methods");
}

void Config::_fill_root_directive(
		http::VirtualServer::Options::Location& location_opts,
		const Config::Directive& root_dir) {
	if (root_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [Filling]: empty directive args: " << root_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	location_opts.file_server_opts.root = root_dir.args.at(0);
	_log.debug(SSTR("[Config] [Filling] fill location root: root=" <<
				location_opts.file_server_opts.root));
}

void Config::_fill_return_directive(
		http::VirtualServer::Options::Location& location_opts,
		const Config::Directive& return_dir) {
	if (return_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [Filling]: empty directive args: " << return_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	const std::string& status_code_str = return_dir.args.at(0);
	int status_code = 0;
	std::istringstream(status_code_str) >> status_code;

	location_opts.handler_type = http::VirtualServer::Options::Location::Return;
	if (return_dir.args.size() == 1) {
		location_opts.return_opts = http::ReturnHandler::Options(
				http::int_to_status_code(status_code));
	} else {
		location_opts.return_opts = http::ReturnHandler::Options(
				http::int_to_status_code(status_code), return_dir.args.at(1));
	}
	_log.debug("[Config] [Filling] fill location return options");
}


void Config::_fill_listen_directive(http::VirtualServer::Options& virtual_server_opts,
		const Config::Directive& listen_dir) {
	if (listen_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [Filling] empty directive args: " << listen_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	// fill addr and port
	{
		const std::string& addr = listen_dir.args.at(0);
		std::size_t colon = addr.find(":");
		if (colon == std::string::npos) {
			std::istringstream(addr) >> virtual_server_opts.port;
		} else {
			virtual_server_opts.addr = addr.substr(0, colon);
			const std::string port_str = addr.substr(colon+1);

			if (!utils::is_number(port_str) || port_str.empty()) {
				_log.fatal(SSTR("[Config] [Filling] bad or empty listen port in addr: " << addr));
				throw FillingBadDirectiveArgsException();
			}
			std::istringstream(port_str) >> virtual_server_opts.port;
		}
		_log.debug(SSTR("[Config] [Filling] fill listener vs: " << virtual_server_opts.addr <<
					":" << virtual_server_opts.port));
	}
	if (listen_dir.args.size() == 1) {
		return;
	}
	// fill listener backlog (if exist)
	{
		const std::string& backlog = listen_dir.args.at(1);
		std::size_t separator = backlog.find_last_of("backlog=");
		if (separator == std::string::npos) {
			_log.fatal(SSTR("[Config] [Filling] bad directive backlog args: " << backlog));
			throw FillingBadDirectiveArgsException();
		} else {
			const std::string backlog_str = backlog.substr(separator+1);

			if (!utils::is_number(backlog_str) || backlog_str.empty()) {
				_log.fatal(SSTR("[Config] [Filling] bad or empty listen backlog: " << backlog_str));
				throw FillingBadDirectiveArgsException();
			}

			std::istringstream(backlog_str) >> virtual_server_opts.listener_backlog;
		}
		_log.debug(SSTR("[Config] [Filling] fill listener backlog: " <<
					virtual_server_opts.listener_backlog));
	}
}

void Config::_fill_error_log_directive(const Config::Directive& logger_dir) {
	if (logger_dir.args.empty()) {
		_log.fatal(SSTR("[Config] [Filling]: empty directive args: " << logger_dir.name));
		throw FillingEmptyDirectiveArgsException();
	}
	// fill log output filename
	{
		_log_opts.file_name = logger_dir.args.at(0);
		_log.debug(SSTR("[Config] [Filling] logger output file: " <<
					_log_opts.file_name));
	}
	if (logger_dir.args.size() == 1) {
		return;
	}
	// fill log message level
	{
		_log_opts.level = logger::str_to_level(logger_dir.args.at(1));
		_log.debug(SSTR("[Config] [Filling] logger message level to: " <<
					logger::level_to_str(_log_opts.level)));
	}
}

const char *Config::FillingEmptyDirectiveArgsException::what() const throw() {
	return "Expect not empty args in directive";
}

const char *Config::FillingBadDirectiveArgsException::what() const throw() {
	return "Expect not empty args in directive";
}

const char *Config::FillingEmptyModuleArgsException::what() const throw() {
	return "Expect not empty args in module";
}

const char *Config::FillingUnknownDirectiveException::what() const throw() {
	return "Unknown directive name";
}

const char *Config::FillingUnknownModuleException::what() const throw() {
	return "Unknown module name";
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
