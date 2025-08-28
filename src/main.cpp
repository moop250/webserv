
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
#include "serverInitialization.hpp"
#include "Debug.hpp"
#include "Server.hpp"

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
	Server	serv(config->getServerData(0));

	return ;
	/* pour les attributs basics en bool ou string on get avec le nom de l'attribut*/
	std::cout << "autoindex : " << (serv.autoindex()? "ON":"OFF") << std::endl;	//	tu peux rajouter le prefixe get devant les fonctions
																				//	si ca t'arrange
	
	/*	Pour les pages d'erreurs :
		* Soit tu get toute la struct tel que
	*/
	ErrorPages	pages = serv.errorPages();
	// puis...
	std::cout << pages.error(/* position (0 by default)*/) << std::endl
		<< pages.path(/* same */) << std::endl
		<< pages.content(1) << std::endl;	//	Ca c'est pour les arguments basics
	
	/* 	Ou du coup :
		* serv.errorPages().error(position);
	*/

	/* Tu peux get direment l'index de ce que tu cherches */
	bool	is_there_an_error_page = pages.has(404);
	if (!is_there_an_error_page)
		std::cout << "there is no error page\n";
	// ou bien en enum :
	is_there_an_error_page = pages.has(REQUEST_ERROR_BAD_REQUEST);	

	/*	avec la methode find aussi */

	int	index = pages.find(404);
	if (index == -1)
		std::cout << "Not found\n";
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

	std::cout << (indloc.autoindex() ? "ON":"OFF");
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
		return (config);
	}
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

	Connection		connection;
	int	fd_client = 10;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\nHost: localhost:8002\r\n\r\n";
	parse_request(connection, *config, fd_client, env);
	// s_ServerData server = config.getServerData(0);

	eventLoop();

	ErrorDebug(dfile, "Event Loop Undefined");

	free(config);
	return 0;
}
