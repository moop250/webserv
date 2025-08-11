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
#include "Debug.hpp"

static void	init()
{
	//	memset 0
	//	check config file if errors
	return ;
}

Config	*parseConfigFile(std::string file, Debug &dfile)
{
	Config	*config = new Config(file);
	
	(void)dfile;
	//	call init
	//	get info from config file
	//	define outputs before launch
	init();
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
	if (ac != 2)
		return Error("Bad Arguments", __func__, __FILE__, __LINE__), -1;

	Debug	dfile;
	Config	*config = parseConfigFile(static_cast<std::string>(av[1]), dfile);

	ErrorDebug(dfile, "Config file parsing uncomplete");

	setUpServer(config);	
	eventLoop();
	free(config);
	return 0;
}
