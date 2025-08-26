
#include <cstdio>
#include <exception>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

/* Following https://ncona.com/2019/04/building-a-simple-server-with-cpp/ to try and get a better understanding of how sockets work*/

#include "Request.hpp"
#include "request_handler.hpp"
#include "Config.hpp"

//int main(int ac, char** av, char **env)
//{
	//(void)ac;
	//(void)av;
	//int				fd_client = 10;
	//Debug			dfile;
	//Config			*c = new Config("configFiles/goodConfigs/simple.config", dfile);
	//Config			config("configFiles/goodConfigs/simple.config", dfile);
	//Connection		connection;
	//connection.body_bytes_read = 0;
	//connection.state = READING_METHOD;
	//connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\nHost: localhost:8002\r\n\r\n";
	//parse_request(connection, config, fd_client, env);
	//// s_ServerData server = config.getServerData(0);
	//config.printServers();
	//std::cout << *c << config
	// << config.getNbServers();
//=======
#include "ConfigError.hpp"
#include "Error.hpp"
#include "Sockets.hpp"
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

void	eventLoop(Config *config, ServerSocket *socket, char **env)
{
	struct	pollfd *fds = initPoll(socket);
	//	execution loop
	while (1) {
		int	pollCount = poll(fds, socket->getTotalSocketCount(), -1);

		if (pollCount == -1) {
			std::cerr << "Error: Poll" << std::endl;
			break;
		}

		incomingConnection(socket, &fds, config, env);
	}
	// 		Lire la requête HTTP
	// 		Parser la requête
	//		...
	//		... ?
	// 		Construire la réponse HTTP
	// 		Envoyer la réponse
	// 		Fermer (ou garder ouverte) la connexion
	delete [] fds;
	delete socket;
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
	ServerSocket *socket = NULL;

	dfile.append("\n\n//////////////////\n// Parsing Part //\n//////////////////");
	if (ac == 2)
		config = parseConfigFile(static_cast<std::string>(av[1]), dfile);
	else
		config = parseConfigFile("configFiles/goodConfigs/default.config", dfile);
	if (!config)
		return (-1);

	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");
	
	try {
		socket = initalizeServer(config);
	} catch (std::exception &e) {
		std::cout << RED << e.what() << RESET << std::endl;
	}

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");

	eventLoop(config, socket, env);

	ErrorDebug(dfile, "Event Loop Undefined");

	free(config);
	return 0;
}
