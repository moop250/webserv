/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/29 13:49:33 by hoannguy         ###   ########.fr       */
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

// untested
void parse_type(Connection& connection) {
	std::string path;
	std::string extension;

	path = connection.getRequest().getPath();
	extension = path.substr(path.rfind('.'));
	if (isCGI(extension) == true) {
		connection.getRequest().setRequestType(CGI);
	} else {
		connection.getRequest().setRequestType(File);
	}
	connection.getRequest().setFileType(extension);
	// check content-type to see if compatible, if not UNSUPPORTED_MEDIA_TYPE
}

// stuffs to do
int parse_request_type(Connection& connection) {
	struct stat	file_stat;
	std::string	path;
	std::string	method;
	int			code;

	// Check if path is cgi /cgi directory or not, pre-append the cgi path or root path
	path = connection.getRequest().getPath();

	method = connection.getRequest().getMethod();
	code = stat(path.c_str(), &file_stat);
	if (code == -1) {
		switch (errno) {
			case EACCES:
				error_response(connection, FORBIDDEN);
				break;
			case ENOENT:									// Not exist
				error_response(connection, NOT_FOUND);
				break;
			case ENOTDIR:									// bad path format
				// fall through
			case ENAMETOOLONG:								// path too long
				error_response(connection, BAD_REQUEST);
				break;
			case ENOMEM:									// out of memory
				error_response(connection, INTERNAL_ERROR);
				break;
		}
		return -1;
	}
	if (S_ISDIR(file_stat.st_mode)) {
		connection.getRequest().setRequestType(Directory);
	} else if (S_ISREG(file_stat.st_mode)) {
		parse_type(connection);
	} else {
		error_response(connection, BAD_REQUEST);
		return -1;
	}
	return 0;
}

int handle_request(Connection& connection, char **env) {
	int	requestType;

	// if (parse_request_type(connection) == -1)
	// 	return -1;
	
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	if (requestType == CGI)
		return CGI_handler(connection, env);
	if (requestType == Directory)
		return directory_handler(connection);
	if (requestType == File)
		return file_handler(connection);
	return -1;
}