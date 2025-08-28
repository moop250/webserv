/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/28 10:21:15 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

int CGI_handler(Connection& connection, char** env) {
	(void)connection;
	(void)env;
	// signal
	// check method with connection.getRequest().getMethod()
	// check cgi type connection.getRequest().getCgiType()
	// fork
	// pipe
	// execve
	// get result to connection.getResponse().setBody()
	return 0;
}

int directory_handler(Connection& connection) {
	(void)connection;

	return 0;
}

int handle_request(Connection& connection, Config& config, char **env) {
	int	requestType;

	(void)config;
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	if (requestType == CGI)
		return CGI_handler(connection, env);
	else if (requestType == Directory)
		return directory_handler(connection);
	else if (requestType == File)
		return file_handler(connection);
	// return -1 here once the config is ok
	return 0;
}