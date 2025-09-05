/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/05 12:36:08 by hoannguy         ###   ########.fr       */
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

std::string generate_name(const std::string& extension) {
	std::time_t	time;
	std::string	name;
	int			random;

	time = std::time(NULL);
	random = std::rand();
	return std::string("upload_"
					+ size_to_string(time)
					+ size_to_string(random)
					+ extension);
}

// Create a file with request body as content.
// File's name is chosen by server.
int post_directory(Connection& connection) {
	std::string		file_name;
	std::string		path;
	std::string		extension;
	std::fstream	file;

	if (connection.getServer().storage().empty())
		path = connection.getRequest().getPath();
	else
		path = connection.getServer().storage();
	std::cout << path << std::endl;
	if (access(path.c_str(), W_OK | X_OK) == -1) {
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
	extension = getExtension(connection.getRequest().getContentType());
	while (true) {
		file_name = generate_name(extension);
		if (access(file_name.c_str(), F_OK) != -1)
			continue ;
		if (path[path.size() - 1] != '/')
			path += '/';
		path += file_name;
		file.open(path.c_str(), std::ios::out | std::ios::binary);
		if (!file.is_open()) {
			error_response(connection, INTERNAL_ERROR);
			return -1;	
		}
		file << connection.getRequest().getBody();
		file.close();
		connection.getResponse().setCode(201);
		connection.getResponse().setCodeMessage("Created");
		
		// Check later to return URI path instead of system path
		connection.getResponse().setHeader("Location", path);
		
		connection.getResponse().constructResponse();
		connection.setState(SENDING_RESPONSE);
		std::cout << connection.getResponse() << std::endl;
		break ;
	}
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