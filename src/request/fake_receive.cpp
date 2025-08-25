/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_receive.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:42:13 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/25 13:55:11 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "request_handler.hpp"

void fake_receive() {
	// remove later ->
	int				fd_client = 10;
	char			**env;
	Debug			dfile;
	Config			config("../../configFiles/goodConfigs/simple.config", dfile);
	// <- remove later


	// Initiate connection struct
	Connection	connection;
	connection.state = READING_METHOD;
	connection.chunked_size = -1;
	// loop to recv() here;

	parse_request(connection, config, fd_client, env);
}