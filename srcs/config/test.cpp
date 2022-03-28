#include <vector>
#include <iostream>
#include "Config.hpp"
#include "../logger/Logger.hpp"


int main(int argc, char **argv)
{
	std::vector<std::string> tokens;
	for (int i = 1; i < argc; ++i)
		tokens.push_back(argv[i]);

	logger::Logger log;
	config::Config conf(log);

	try
	{
		conf._parsing(tokens);
	}
	catch (std::exception& e)
	{
		log.fatal(e.what());
	}

	return (0);
}