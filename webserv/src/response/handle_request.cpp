/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/03 15:47:16 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

// stuffs to do
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
	// remake
	// if (getMIMEType(extension).empty()) {
	// 	error_response(connection, UNSUPPORTED_MEDIA_TYPE);
	// 	return -1;
	// }
	connection.getRequest().setFileType(extension);
	return 0;
}

int parse_request_type(Connection& connection) {
	struct stat	file_stat;
	std::string	path;
	int			code;

	path = connection.getRequest().getPath();
	if (path[0] == '/')
		path.erase(0, 1);
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

// get the location, replace the location with root if root is not empty
void path_merge(Connection& connection) {
	std::string	path;
	std::string	root;
	std::string	location;
	
	path = connection.getRequest().getPath();
	root = connection.getServer().root();
	if (root.empty())
		return;
	if (root[root.size() - 1] == '/')
		root.erase(root.size() - 1, 1);
	location = connection.getServer().getLocation();

	std::cout << "LOCATION: " <<  location << std::endl;
	
	if (location.empty()) {
		path.insert(0, root);
		connection.getRequest().setPath(path);
	} else {
		if (location[0] == '/')
			location.erase(0, 1);
		path.replace(0, location.size(), root);
		connection.getRequest().setPath(path);
	}
	std::cout << "PATH: " << path << std::endl;
}

int handle_request(Connection& connection) {
	int	requestType;

	if (connection.getServer().getLocation() == "/cgi/")
	path_merge(connection);
	if (parse_request_type(connection) == -1)
		return -1;
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	if (requestType == CGI)
		return CGI_handler(connection);
	if (requestType == Directory)
		return directory_handler(connection);
	if (requestType == File)
		return file_handler(connection);
	return -1;
}