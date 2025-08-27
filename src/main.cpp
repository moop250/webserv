
#include <cstdio>
#include <exception>
#include <stdexcept>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

/* Following https://ncona.com/2019/04/building-a-simple-server-with-cpp/ to try and get a better understanding of how sockets work*/

#include "Request.hpp"
#include "Connection.hpp"
#include "Response.hpp"
#include "request_handler.hpp"
#include "Config.hpp"
#include "ConfigError.hpp"
#include "Error.hpp"
#include "serverInitialization.hpp"
#include "Debug.hpp"

Config	*parseConfigFile(std::string file, Debug &dfile)
{
	Config	*config;
	std::stringstream	msg;
	
	config = new Config(file, dfile);
	config->parseContent();
	config->sanitize();
	msg << *config;
	dfile.append(msg.str().c_str());
	ConfigError	error(*config);
	if (error.isConfigValid())
		return (config);

	std::cerr << RED << "Program stopped\n" << RESET;
	delete config;
	return NULL;
}

void	setUpServer(Config *config)
{
	//	sockets
	//	ports
	//	Methods
	//	adrrinfo
	//	...
	(void)config;
	return ;
}

void	eventLoop()
{
	//	execution loop
	// 		Wait connection (socket)
	// 		Accepter la connexion (si nouveau client)
	// 		Lire la requête HTTP
	// 		Parser la requête
	//		...
	//		... ?
	// 		Construire la réponse HTTP
	// 		Envoyer la réponse
	// 		Fermer (ou garder ouverte) la connexion
	return ;
}

void	free(Config *conf)
{
	//	free all
	delete conf;
	return ;
}

int main(int ac, char** av, char **env)
{
	Debug	dfile("General.log");
	Config	*config = NULL;
	ServerSocket socket;

	dfile.append("\n\n//////////////////\n// Parsing Part //\n//////////////////");
	if (ac == 2)
		config = parseConfigFile(static_cast<std::string>(av[1]), dfile);
	else
		config = parseConfigFile("configFiles/goodConfigs/default.config", dfile);
	if (!config)
		return (-1);

	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");

	setUpServer(config);
	try {
	} catch (std::exception &e) {
		std::cout << RED << e.what() << RESET << std::endl;
	}

	ErrorDebug(dfile, "Server Setup Incomplete");

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");

	Connection		connection(10);
	connection.buffer = "POST /cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost:8002\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Transfer-Encoding: chunked\r\n"
						"\r\n"
						"D\r\n"
						"Hello World!\n\r\n"
						"20\r\n"
						"This is a greeting from Lausanne\r\n"
						"0\r\n"
						"\r\n";
	parse_request(connection, *config, env);
	// s_ServerData server = config.getServerData(0);

	eventLoop();

	ErrorDebug(dfile, "Event Loop Undefined");

	free(config);
	return 0;
}
