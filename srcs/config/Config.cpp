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

void    Config::tokenizer(const std::string &line)
{
    std::string token;
    int i = 0;
    int size = line.length();

    while (i < size)
    {
        char c = line[i];
        switch (c) {
            case ' ':
            case '\n':
            case '\t':
            case '\0':
            {
                if (token != "")
                {
                    tokens.push_back(token);
                    token.clear();
                }
                break;
            }
            case '{':
            case '}':
            case ';': {
            if (token != "")
            {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(line.substr(i, 1));
            break ; }
            default:
            token += line[i];
        }
        i++;
    }
}

const   std::string    Config::readFile(const char *filename) {
    std::string					line;
    std::string					buf;
    std::string::size_type      start_comment = 0;
    std::string::size_type      end_comment = 0;

    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw config::Config::FileNotFoundException();
    }
    while (std::getline(input_file, buf))
    {
           buf += '\n';
           line += buf;
    }
    input_file.close();
    while (start_comment != std::string::npos)
    {
        start_comment = line.find_first_of('#');
        end_comment = line.find_first_of('\n', start_comment);
        if (start_comment != std::string::npos)
            line = line.erase(start_comment, end_comment - start_comment);
    }
    return (line);
}

const std::vector<std::string> Config::_lexing(const std::string& filename) {
	_log.debug("start procesing a file: " + filename);
	_log.debug("lexing: read all file, delete comments  and splitting words into tokens");
	tokenizer(readFile(filename.c_str()));
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
