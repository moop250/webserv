
#include <cstdio>
#include <exception>
#include <map>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <vector>

/* Following https://ncona.com/2019/04/building-a-simple-server-with-cpp/ to try and get a better understanding of how sockets work*/

#include "Request.hpp"
#include "Connection.hpp"
#include "Response.hpp"
#include "request_handler.hpp"
#include "Config.hpp"
#include "ConfigError.hpp"
#include "Error.hpp"
#include "Sockets.hpp"
#include "Debug.hpp"
#include "Server.hpp"
#include "RequestServer.hpp"
/*	Error pages ANNEXE : 
	* enum RequestError
	* {
	* 	ERROR_BAD_REQUEST,
	* 	ERROR_NOT_FOUND,
	* 	ERROR_NOT_ALLOWED,
	* 	ERROR_LENGTH,
	* 	ERROR_TOO_LARGE,
	* 	ERROR_NOT_IMPLEMENTED,
	* 	ERROR_HTTP_MISSMATCH,
	* 	ERROR_INTERNAL_ERROR,
	* 	OTHER
	* };
	*
	* for nguyen
*/

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

// add	serv.findLoc();

void	eventLoop(Config *config, ServerSocket *socket)
{
	t_fdInfo fdInfo;
	initPoll(socket, &fdInfo);
	std::map<int, Connection> connectMap;

	try {
		while (1) {
			int	pollCount = poll(&fdInfo.fds[0], socket->getTotalSocketCount(), -1);

			if (pollCount == -1) {
				std::cerr << "Error: Poll" << std::endl;
				break;
			}

			incomingConnection(socket, &fdInfo, config, &connectMap);
		}
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}

	delete socket;
	return ;
}

void	free(Config *conf)
{
	//	free all
	delete conf;
	return ;
}

int main(int ac, char** av)
{
	Debug	dfile("General.log");
	Config	*config = NULL;
	ServerSocket *socket = NULL;

	std::srand(std::time(NULL));
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
		std::cerr << RED << e.what() << RESET << std::endl;
		delete config;
		return (-2);
	}

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");
	
	try {
		eventLoop(config, socket);
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		delete config;
		delete socket;
		return (-3);	
	}

	// Connection		connection;
	// connection.buffer = "POST http://www.test.com/cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
	// 					"Host: localhost1:8001\r\n"
	// 					"Connection: Keep-Alive\r\n"
	// 					"Keep-Alive: timeout=5, max=200\r\n"
	// 					"Content-Length: 12\r\n"
	// 					"\r\n"
	// 					"Hello World!";
	// parse_request(connection, *config, env);

	ErrorDebug(dfile, "Event Loop Undefined");

	delete config;
	return 0;
}
