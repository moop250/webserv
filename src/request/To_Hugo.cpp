/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   To_Hugo.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:42:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/29 13:55:53 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "request_handler.hpp"

void To_Hugo() {
	// remove later ->
	char	**env;
	Debug	dfile;
	Config	config("../../configFiles/goodConfigs/simple.config", dfile);
	// <- remove later


	// Yo Hugo, voici le flow comme je vois. C'est pas testé donc pour référence seulement.
	
	// 1.	Tu peux commencer par initialiser l'objet Connection et un code.
	// 		Il prend un int qui est fd du client, à voir encore si nécessaire de l'envoyer
	//		L'attribute buffer est public. Pour stocker les données de recv().
	//	 	L'attribute state est privé. Pour checker l'état de la connection.
	int			code = 0;
	Connection	connection();
	
	// 2.	Mets le state en READING_METHOD
	connection.setState(READING_METHOD);
	
	// 3.	Tu peux commencer à loop pour recv(). Pour chaque loop t'appelles parse_request()
	// 		Après chaque parse_request() call, le state de Connection est mis à jour.
	// 		Il faut donc gérer la connection en fonction du state.
	//		Si code == CONTINUE_READ, la requête n'est pas complète.
	// 		Si code == MAKING_RESPONSE, la requête est reçue en entier.
	//		Si code == -1, il y a un HTTP erreur.
	while (code == CONTINUE_READ) {
		// recv();
		code = parse_request(connection, config, env);
	}

	// 4.	Si la requête est reçue en entier, on va gérer le Keep-Alive.
	//		Vérifier si la Connection est keep-alive, close ou rien.
	// 		Si keep-alive, il faut chercher les parameters getKeepAliveTimeout() et getKeepAliveMax() puis gérer en fonction.
	//		Si close, fermes la connection après avoir envoyé la réponse.
	//		Si rien, maintiens la connection.
	if (code == MAKING_RESPONSE) {
		int	timeout;
		int	max;
		std::string keepAlive;
	
		keepAlive = connection.getRequest().getKeepAlive();
		if (keepAlive == "keep-alive") {
			timeout = connection.getRequest().getKeepAliveTimeout();
			max = connection.getRequest().getKeepAliveMax();
			if (timeout != -1) {
				// Do stuffs
			}
			if (max != -1) {
				// Do stuffs
			}
		}
		if (keepAlive == "close") {
			// Do stuffs
		}
	}

	// 5.	Si le code est -1, fermes la connection après avoir envoyé la réponse.
	if (code == -1) {
		// Do stuffs
	}

	// 6. Call handle_request
	if (handle_request(connection, env) == -1)
		return -1;
	
	// 7.	Envoyer la réponse, à voir encore avec le POLLOUT si c'est mieux d'envoyer ici ou ailleur.

	// 8.	Remets le state en waiting request.
	connection.setState(WAITING_REQUEST);
}