#include "logger/Logger.hpp"
#include "config/Config.hpp"
#include "server/Server.hpp"

int main(int argc, char *argv[]) {
	logger::Logger log;
	config::Config conf(log);
	server::Server serv(log);

	// Check that number of command line arguments is strictly one - name
	//  of configuration file of a web server
	if (argc != 2) {
		log.fatal("wrong command argument number");  //TODO better message
		return (1); // TODO error codes put in define or constant enum
	}

	// Parse configuration file
	try {
		conf.parse(argv[1]);
	}
	catch (std::exception &e) { // TODO catch specific exeption set
		log.fatal(e.what());  //TODO better message
		return (2); // TODO error codes put in define or constant
	}

	// Set up logger and server according to configuration file
//	log.set_up(conf.get_logger());
	serv.set_up(conf.get_server());

	// Serve HTTP connections
	try {
		// graceful closing in case of interapt signal
		serv.serve_http();
	}
	// Walk into this catch block only in case of internal server fatal error
	catch (...) { // TODO specific exeption set
		log.fatal("internal server error");  //TODO better message
		return (3); // TODO error codes put in define or constant
	}
	return (0); // TODO error codes put in define or constant enum
}
