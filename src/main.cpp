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

static void	init()
{
	//	memset 0
	//	check config file if errors
	return ;
}

void	parseConfigFile()
{
	//	call init
	//	get info from config file
	//	define outputs before launch
	return ;
}

void	setUpServer(Config config)
{
	//	sockets
	//	ports
	//	Methods
	//	adrrinfo
	//	...
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

void	free()
{
	//	free all
	return ;
}

int main(int ac, char** av)
{
	class	config = parseConfigFile();

	setUpServer(config);	
	eventLoop(config);
	free();
	return 0;
}
