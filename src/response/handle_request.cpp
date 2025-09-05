/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/05 14:45:54 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

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

// untested
int parse_type(Connection& connection) {
	std::string path;
	std::string extension;

	path = connection.getRequest().getPath();
	extension = path.substr(path.rfind('.'));
	if (isCGI(extension) == true) {
		connection.getRequest().setRequestType(CGI);
	} else {
		connection.getRequest().setRequestType(File);
	}
	// if (getMIMEType(extension).empty()) {
	// 	error_response(connection, UNSUPPORTED_MEDIA_TYPE);
	// 	return -1;
	// }
	connection.getRequest().setFileType(extension);
	return 0;
}

// stuffs to do
int parse_request_type(Connection& connection) {
	struct stat	file_stat;
	std::string	path;
	int			code;

	// Check if path is cgi /cgi directory or not, pre-append the cgi path or root path
	path = connection.getRequest().getPath();

	code = stat(path.c_str(), &file_stat);
	if (code == -1) {
		switch (errno) {
			case ENOENT:									// Not exist
				error_response(connection, NOT_FOUND);
				break;
			case ENOTDIR:									// bad path format
				// fall through
			case ENAMETOOLONG:								// path too long
				error_response(connection, BAD_REQUEST);
				break;
			default:
				error_response(connection, INTERNAL_ERROR);
				break;
		}
		return -1;
	}
	if (S_ISDIR(file_stat.st_mode)) {
		connection.getRequest().setRequestType(Directory);
		return 0;
	} else if (S_ISREG(file_stat.st_mode)) {
		return parse_type(connection);
	} else {
		error_response(connection, BAD_REQUEST);
		return -1;
	}
}

int handle_request(Connection& connection, char **env) {
	int	requestType;

	if (parse_request_type(connection) == -1)
		return -1;
	
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	if (requestType == CGI)
		return CGI_handler(connection, env);
	if (requestType == Directory)
		return directory_handler(connection);
	if (requestType == File)
		return file_handler(connection);
	return -1;
}