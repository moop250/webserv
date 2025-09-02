
#include <cstdio>
#include <exception>
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
static void	testServer(Config *config)
{
	return ;
	Server	serv(config->getServerData(0));

	/* pour les attributs basics en bool ou string on get avec le nom de l'attribut*/
	std::cout << "autoindex : " << (serv.autoindex()? "ON":"OFF") << std::endl;	//	tu peux rajouter le prefixe get devant les fonctions		
	//	si ca t'arrange
	if (serv.undefined(UPLOAD_STORAGE))
		std::cout << "server has not storage\n";
	else
		std::cout << "Server storage is defined\n";
	if (serv.has("ADD"))
		std::cout << "server allows method ADD\n";
	if (serv.has("YAYA"))
		std::cout << "YAYA is not a part of the server\n";							
	/*	Pour les pages d'erreurs :
		* Soit tu get toute la struct tel que
	*/
	ErrorPages	pages = serv.errorPages();

	std::string c = pages.content(REQUEST_ERROR_BAD_REQUEST);
	std::cout << CYAN << c << RESET << std::endl;
	// puis...
	std::cout << pages.error(/* position (0 by default)*/) << std::endl
		<< pages.path(/* same */) << std::endl
		<< pages.content(1) << std::endl;	//	Ca c'est pour les arguments basics
	
	/* 	Ou du coup :
		* serv.errorPages.error(position);
		* ou serv.errorPages.error(404)
		* ou serv.errorPages.error(serv.errorPages.find(404))
	*/

	/* Tu peux get direment l'index de ce que tu cherches */
	bool	is_there_an_error_page = pages.has(404); // or NOT_FOUND du coup
	if (!is_there_an_error_page)
		std::cout << "there is no error page\n";
	// ou bien en enum :
	is_there_an_error_page = pages.has(REQUEST_ERROR_BAD_REQUEST);	

	/*	avec la methode find aussi */
//	if (index == -1)
//		std::cout << "Not found\n";
	//	ou bien :
	std::string content = pages.content(static_cast<requestType>(404));
	// 	comme pour
	content = pages.content(REQUEST_ERROR_HTTP_MISSMATCH);

	/*	Les locations se get a peu pres de la meme maniere
		* soit un vecteur de toutes les locations : 	
	*/
	std::vector<Location>	loc = serv.locations();
	// soit par le bon index
	Location	indloc = serv.location(0);

	std::cout << "Location" << serv.location().path() << std::endl;
	std::cout << (indloc.autoindex() ? "ON":"OFF") << std::endl;
	return ;
}

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
	{
		testServer(config);
		//RequestServer	rs(*config, "localhost2", "8002", "/zbru");	//	default server values 
		//std::cout << rs;
		//std::cout << "Server is : " << (rs.isValid() ? "Valid" : "Not Valid") << std::endl;
		return (config);
	}
	std::cerr << RED << "Program stopped\n" << RESET;
	delete config;
	return NULL;
}

// add	serv.findLoc();

void	eventLoop(Config *config, ServerSocket *socket, char **env)
{
	std::vector<pollfd> fds = initPoll(socket);
	try {
		while (1) {
			int	pollCount = poll(&fds[0], socket->getTotalSocketCount(), -1);

			if (pollCount == -1) {
				std::cerr << "Error: Poll" << std::endl;
				break;
			}

			incomingConnection(socket, &fds, config, env);
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

int main(int ac, char** av, char **env)
{
	Debug	dfile("General.log");
	Config	*config = NULL;
	// ServerSocket *socket = NULL;

	(void)env;

	dfile.append("\n\n//////////////////\n// Parsing Part //\n//////////////////");
	if (ac == 2)
		config = parseConfigFile(static_cast<std::string>(av[1]), dfile);
	else
		config = parseConfigFile("configFiles/goodConfigs/default.config", dfile);
	if (!config)
		return (-1);

	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");
	
	// try {
	// 	socket = initalizeServer(config);
	// } catch (std::exception &e) {
	// 	std::cerr << RED << e.what() << RESET << std::endl;
	// 	delete config;
	// 	return (-2);
	// }

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");
	
	// eventLoop(config, socket, env);

	Connection		connection;
	connection.buffer = "POST http://www.test.com/cgi/test.java?user=Nguyen&school=42 HTTP/1.1\r\n"
						"Host: localhost1:8001\r\n"
						"Connection: Keep-Alive\r\n"
						"Keep-Alive: timeout=5, max=200\r\n"
						"Content-Length: 12\r\n"
						"\r\n"
						"Hello World!";
	parse_request(connection, *config, env);

	ErrorDebug(dfile, "Event Loop Undefined");

	delete config;
	return 0;
}
