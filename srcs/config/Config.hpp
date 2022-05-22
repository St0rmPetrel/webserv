#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include "IConfig.hpp"
#include "../logger/Logger.hpp"

#include "../logger/Options.hpp"
#include "../server/Options.hpp"
#include "Config.hpp"

namespace config {
	// Config class responsible for parsing of a configuration file and returning options
	// of other classes of the program
	class Config : public IConfig {
	private:
		// Directive data structure for "directive" in configuration file
		// example:
		//   Directive{ name: "error_log", args: {"/dev/stderr","warn"} }
		struct Directive {
			std::string name;
			std::vector<std::string> args;
		};
		// Module data structure for "module", "block" or "context" in configuration file
		// example:
		//   Module{
		//     name: "global",
		//     directive: {},
		//     modules{
		//       Module{
		//         name: "http",
		//         directive: {},
		//         modules{ Module{name: "server", ...}, Module{name: "server", ...} }
		//       }
		//     }
		//   }
		struct Module {
			std::string name;
			std::vector<Module> modules;
			std::vector<Directive> directives;
			std::vector<std::string> args;
		};
	private:
		// _serv_opts options of a Server, Server is a class of the program
		// which is responsible for network connection and HTTP protocol
		server::Options _serv_opts;
		// _log_opts options of a Logger, Logger is a class of the program
		// which is responsible for printing or recording information about
		// progress of the program
		logger::Options _log_opts;

		const logger::Logger &_log;

		std::vector<std::string> _tokens;
	public:
		Config(const logger::Logger &log);

		~Config();

		// parse take name of configuration file and read it
		// fill _serv_opts and _log_opts according content of the file
		// if syntax of the file is bad, throw exception
		void parse(const std::string &filename);

		// get_logger get Logger options
		const logger::Options &get_logger() const;

		// get_server get Server options
		const server::Options &get_server() const;

	public:
		struct FileNotFoundException : public std::exception {
			const char *what() const throw();
		};

		struct ReadingConfigFileException : public std::exception {
			const char *what() const throw();
		};

		struct ParsingDirException : public std::exception {
			const char *what() const throw();
		};

		struct ParsingModuleException : public std::exception {
			const char *what() const throw();
		};

		struct ParsingBraceException : public std::exception {
			const char *what() const throw();
		};

	private:
		// _lexing read configuration file by it name ignoring strings which start
		// with "#" (comments) and splits received text on tokens
		//   example:
		//   ```
		//   # Some comment
		//   daemon off;
		//   events {
		//   }
		//   ``` -> {"daemon", "off", ";", "events", "{", "}"}
		// May throw exception in case of bad syntax of configuration file
		//   example of bad syntax: "daemon off;;", "events {{ }", "events {; }"
		//   (wrong token sequence)
		const std::vector<std::string> _lexing(const std::string &filename);

		const std::string _readFile(const char *filename);

		void _tokenizer(const std::string &line);

		// _parsing sort tokens into Module structure
		//   example:
		//     {"daemon", "off", ";", "events", "{", "}"}
		//   ->
		//     Module{
		//       name: "main",
		//       directives: { Directive{name:"", args:{"off"}} },
		//       modules: {
		//         Module {name: "events", directives: {}, modules: {}}
		//       }
		//     }
		// May throw exception in case of bad syntax
		//   example of bad syntax: {"daemon", "off", ";", "events", "{"}
		//   (don't close curly bracket)
		void _parsing(const std::vector<std::string> &tokens);

		Directive _collect_directive(const std::vector<std::string> &tokens) const;

		Module _collect_module(const std::vector<std::string> &name, const std::vector<std::string> &tokens,
							   std::vector<std::string>::const_iterator &it, bool is_global) const;

		// _fill_options sort data from Module structure to options of other classes
		// will be expend during the progress of creation program
		// May throw exception in case of bad directive or module name,
		// bad module nesting or bad arguments in directive
		void _fill_options(const Module &module);

	private:
		Module _global_module;

    public:
        std::string Print(void);
    private:
        std::string _print_dir(const Directive& dir, int indent);
        std::string _print_module(const Module& mod, int indent);
	};
};

#endif
