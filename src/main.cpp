#include <cstdio>
#include <exception>
#include <stdexcept>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

/* Following https://ncona.com/2019/04/building-a-simple-server-with-cpp/ to try and get a better understanding of how sockets work*/

#include "Config.hpp"
#include "Error.hpp"
#include "serverInitialization.hpp"
#include "Debug.hpp"

Config	*parseConfigFile(std::string file, Debug &dfile)
{
	Config	*config;
	
	try
	{
		config = new Config(file, dfile);
	}
	catch(const std::exception& e)
	{
		Error("here at : ", __func__, __FILE__, __LINE__);
		throw Config::BadFileException();
	}
	config->parseContent();
	try
	{
		std::stringstream	strparam;
		strparam << *config;
		dfile.append(strparam.str().c_str());
		dfile.append("Config printed");
	}
	catch(const std::exception& e)
	{
		Error("Config", __func__, __FILE__, __LINE__);
		throw Config::MissingParamException();
	}
	return config;
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

int main(int ac, char** av)
{
	Debug	dfile;
	Config	*config = NULL;
	ServerSocket socket;

	dfile.append("\n\n//////////////////\n// Parsing Part //\n//////////////////");
	if (ac == 2)
		config = parseConfigFile(static_cast<std::string>(av[1]), dfile);
	else
		config = parseConfigFile("configFiles/default.config", dfile);

	std::cout << "End pars\n";
	ErrorDebug(dfile, "Config file parsing uncomplete");

	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");

	setUpServer(config);
	try {
		socket = initalizeServer(config);
	} catch (std::exception &e) {
		std::cout << RED << e.what() << std::endl;
	}

	std::cout << socket.getSocketCount() << std::endl;
	ErrorDebug(dfile, "Server Setup Incomplete");

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");

	eventLoop();

	ErrorDebug(dfile, "Event Loop Undefined");

	free(config);
	return 0;
}
