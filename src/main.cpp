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

Config	*parseConfigFile(std::string file, Debug &dfile)
{
	Config	*config;
	
	try
	{
		config = new Config(file, dfile);
	}
	catch(const std::exception& e)
	{
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
		Error("Config failed");
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

	dfile.append("\n\n//////////////////\n// Parsing Part //\n//////////////////");
	if (ac == 2)
		config = parseConfigFile(static_cast<std::string>(av[1]), dfile);
	else
		config = parseConfigFile("default.config", dfile);

	ErrorDebug(dfile, "Config file parsing uncomplete");

	dfile.append("\n\n//////////////////\n//  Setup Part  //\n//////////////////");

	setUpServer(config);
	
	ErrorDebug(dfile, "Server Setup Incomplete");

	dfile.append("\n\n//////////////////////\n// Event loop start //\n//////////////////////");

	eventLoop();

	ErrorDebug(dfile, "Event Loop Undefined");

	free(config);
	return 0;
}
