/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/29 23:00:02 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

// auto-index on -> return the list of files in that directory in html
// index specified -> return index.html of that location/server
// both auto-index off and no index specified -> 403 Fobidden
int get_directory(Connection& connection) {
	(void)connection;
	return 0;
}

// Create a file with request body as content. File's name is chosen by server.
int post_directory(Connection& connection) {
	std::string	file_name;
	std::string	path;
	std::string parent_directory;

	path = connection.getRequest().getPath();
	parent_directory = path.substr(0, path.rfind("/"));
	if (access(parent_directory.c_str(), W_OK | X_OK) == -1) {
		switch (errno) {
			case EACCES:
				error_response(connection, FORBIDDEN);
				break ;
			default:
				error_response(connection, INTERNAL_ERROR);
				break;
		}
		return -1;
	}
	// not finish
	return 0;  
}

// Reject with 403 Fobidden if Delete request targets directory
int delete_directory(Connection& connection) {
	error_response(connection, FORBIDDEN);
	return -1;
}

int directory_handler(Connection& connection) {
	std::string	method;

	method = connection.getRequest().getMethod();
	if (method == "GET")
		return get_directory(connection);
	else if (method == "POST")
		return post_directory(connection);
	else if (method == "DELETE")
		return delete_directory(connection);
	return -1;
}