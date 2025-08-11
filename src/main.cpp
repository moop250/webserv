/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlibine <hlibine@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:03:20 by hlibine           #+#    #+#             */
/*   Updated: 2025/05/28 15:57:10 by hlibine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Error.hpp"
#include "Debug.hpp"

static void	init()
{
	//	memset 0
	//	check config file if errors
	return ;
}

Config	*parseConfigFile(const char *file)
{
	Config	*config = new Config();

	(void)file;
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
	
	Config	*config = parseConfigFile(av[1]);
	Debug	dfile;

	ErrorDebug(dfile, "Config file parsing uncomplete");
	setUpServer(config);	
	eventLoop();
	free(config);
	return 0;
}
