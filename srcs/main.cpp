#include "logger/Logger.hpp"
#include "config/Config.hpp"
#include "server/Server.hpp"
#include "utils/utils.hpp"

int main(int argc, char *argv[]) {
	logger::Logger log;
	config::Config conf(log);

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

	// Serve HTTP connections
	try {
		server::Server serv(log, conf.get_server());
		// graceful closing in case of interapt signal
		serv.listen_and_serve();
	}
	// Walk into this catch block only in case of internal server fatal error
	catch (const std::exception& ex) { // TODO specific exeption set
		log.fatal(SSTR("internal server error: " << ex.what()));  //TODO better message
		return (3); // TODO error codes put in define or constant
	}
	return (0); // TODO error codes put in define or constant enum
}
