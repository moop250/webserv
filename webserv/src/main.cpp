
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


std::vector<pollfd> g_fds;
volatile sig_atomic_t g_running = 1; // 1 = en cours, 0 = stop

static void close_all(int sig)
{
    (void)sig;
    g_running = 0;

    for (std::vector<pollfd>::iterator i = g_fds.begin(); i != g_fds.end(); i++) {
        if (i->fd != -1) {
            close(i->fd);
            i->fd = -1;
        }
    }
    std::cerr << "[INFO] SIGQUIT reçu, sockets fermés" << std::endl;
}

void	eventLoop(Config *config, ServerSocket *socket)
{
	t_fdInfo fdInfo;

	initPoll(socket, &fdInfo);

	std::map<int, Connection> connectMap;

	signal(SIGQUIT, close_all);
	g_fds = fdInfo.fds;
	try {
		while (1) {
			int	pollCount = poll(&fdInfo.fds[0], socket->getTotalSocketCount(), 0);

			 if (!g_running)
                break;
			if (pollCount == -1) {
				std::cerr << "Error: Poll" << std::endl;
				break;
			}

			if (pollCount > 0)
				incomingConnection(socket, &fdInfo, config, &connectMap);
			if (pollCount == 0)
				handleTimeout(&fdInfo);
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
		config = parseConfigFile("ressources/configFiles/goodConfigs/default.config", dfile);
	if (!config)
		return (-1);
	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");

	// try {
		// socket = initalizeServer(config);
	// } catch (std::exception &e) {
		// std::cerr << RED << e.what() << RESET << std::endl;
		// delete config;
		// return (-2);
	// }
// 
	// dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");
	// 
	// try {
		// eventLoop(config, socket);
	// } catch (std::exception &e) {
		// std::cerr << RED << e.what() << RESET << std::endl;
		// delete config;
		// delete socket;
		// return (-3);
	// }


	Connection		connection;
	connection.setPort("8001");
	connection.setIP("127.0.0.1");

	// // cpp
	// connection.buffer = "POST /cgi/RPN.cpp HTTP/1.1\r\n"
	// 					"Host: localhost1:8001\r\n"
	// 					"Connection: Keep-Alive\r\n"
	// 					"Keep-Alive: timeout=5, max=200\r\n"
	// 					"Content-Length: 9\r\n"
	// 					"\r\n"
	// 					"1 1 + 6 *";

	// java
	connection.buffer = "POST /cgi/CGI HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 11\r\n"
						"\r\n"
						"Lausanne 42";

	// // GET file
	// connection.buffer = "GET /configFiles/goodConfigs/simple.config HTTP/1.1\r\n"
	// 				"Host: localhost1:8001\r\n"
	// 				"Connection: Keep-Alive\r\n"
	// 				"Keep-Alive: timeout=5, max=200\r\n"
	// 				"\r\n";

	int code = parse_request(connection, *config);
	// std::cout << code << std::endl;
	if (code != -1) {
		code = handle_request(connection);
		// std::cout << code << std::endl;
	try {
		socket = initalizeServer(config);
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		delete config;
		return (-2);
	}
	std::cout << "RESPONSE:\n" << connection.getResponse() << std::endl;

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");
	
	// try {
		// eventLoop(config, socket);
	// } catch (std::exception &e) {
		// std::cerr << RED << e.what() << RESET << std::endl;
		// delete config;
		// delete socket;
		// return (-3);
	// }
// 

	delete config;
	return 0;
	(void)socket;
}
}
