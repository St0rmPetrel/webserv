#include <iostream>
#include "../Config.hpp"

// todo: change path vars
const std::string valid = "/Users/nastya/Desktop/webserv/configs/basic.conf";
const std::string valid_with_comments = "/Users/nastya/Desktop/webserv/configs/basic_with_comments.conf";
const std::string empty = "/Users/nastya/Desktop/webserv/configs/empty.conf";
const std::string invalid = "/Users/nastya/Desktop/webserv/configs/invalid.conf";
const std::string invalid2 = "/Users/nastya/Desktop/webserv/configs/invalid2.conf";
const std::string invalid3 = "/Users/nastya/Desktop/webserv/configs/invalid3.conf";
const std::string invalid4 = "/Users/nastya/Desktop/webserv/configs/no_rights.conf";

int main(void) {
	logger::Options opt;
//	opt.enabled_level = logger::INFO;
	opt.enabled_level = logger::DEBUG;
	logger::Logger log;
	log.set_up(opt);

	// todo: check parsing results
	{
		/// completely valid testcase
		try {
			config::Config config(log);
			config.parse(valid);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// completely valid testcase + comments in conf file
		try {
			config::Config config(log);
			config.parse(valid_with_comments);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// ?
		try {
			config::Config config(log);
			config.parse(empty);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// expected exception - empty module
		try {
			config::Config config(log);
			config.parse(invalid);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// expected exception - double ;
		try {
			config::Config config(log);
			config.parse(invalid2);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// expected exception - ; following {
		try {
			config::Config config(log);
			config.parse(invalid3);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}
	{
		/// unable to open conf file
		try {
			config::Config config(log);
			config.parse(invalid4);
		}
		catch (std::exception &e) {
			log.fatal(e.what());
		}
	}

	return (0);
}
