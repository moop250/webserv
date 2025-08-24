/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:03:20 by hlibine           #+#    #+#             */
/*   Updated: 2025/08/22 14:58:49 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Config.hpp"

int main(int ac, char** av, char **env)
{
	(void)ac;
	(void)av;
	int				fd_client = 10;
	Debug			dfile;
	Config			*c = new Config("configFiles/goodConfigs/simple.config", dfile);
	Config			config("configFiles/goodConfigs/simple.config", dfile);
	Connection		connection;
	connection.body_bytes_read = 0;
	connection.state = READING_METHOD;
	connection.buffer = "GET /wtfwtf?user=Nguyen&school=42 HTTP/1.1\r\nHost: localhost:8002\r\n\r\n";
	parse_request(connection, config, fd_client, env);
	// s_ServerData server = config.getServerData(0);
	config.printServers();
	std::cout << *c << config
	 << config.getNbServers();
	return 0;
}
