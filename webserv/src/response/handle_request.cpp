/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/08 13:38:30 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

int parse_request_type(Connection& connection) {
	struct stat	file_stat;
	std::string	path;
	std::string	extension;
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
	} else if (S_ISREG(file_stat.st_mode)) {
		size_t	pos = path.rfind('.');
		pos == std::string::npos ? extension = "binary" : extension = path.substr(path.rfind('.'));
		connection.getRequest().setRequestType(File);
		connection.getRequest().setFileType(extension);
		if (extension == "binary") {
			extension = "";
		}
	} else {
		error_response(connection, BAD_REQUEST);
		return -1;
	}
	return 0;
}

// get the location, replace the location with root if root is not empty
void path_merge_non_cgi(Connection& connection) {
	std::string	path;
	std::string	root;
	std::string	location;
	
	path = connection.getRequest().getPath();
	root = connection.getServer().root();
	location = connection.getServer().getLocation();
	if (root.empty())
		return;
	if (root[root.size() - 1] == '/')
		root.erase(root.size() - 1, 1);	
	if (location.empty()) {
		path.insert(0, root);
		connection.getRequest().setPath(path);
	} else {
		if (location[location.size() - 1] == '/')
			location.erase(location.size() - 1, 1);
		path.replace(0, location.size(), root);
		connection.getRequest().setPath(path);
	}
}

int path_merge_cgi(Connection& connection) {
	std::string::size_type				extension_pos;
	std::string							path;
	std::string							extension;
	std::string							root;
	std::string							cgi_path;
	std::string							location;
	std::map<std::string, std::string>	cgi;

	path = connection.getRequest().getPath();
	extension_pos = path.rfind('.');
	if (extension_pos == std::string::npos) {
		error_response(connection, BAD_REQUEST);
		return -1;
	}
	extension = path.substr(extension_pos);
	cgi = connection.getServer().cgi();
	if (cgi.count(extension) == 1) {
		cgi_path = cgi[extension];
	}
	if (!cgi_path.empty()) {
		connection.getRequest().setRequestType(CGI);
		connection.getRequest().setFileType(extension);
		root = connection.getServer().root();
		if (path[0] == '/')
			path.erase(0, 1);
		location = connection.getServer().getLocation();
		if (location == "/cgi/")
			location.erase(location.size() - 1, 1);
		path = root + cgi_path;
		connection.getRequest().setPath(path);
		return 0;
	} else {
		error_response(connection, UNSUPPORTED_MEDIA_TYPE);
		return -1;
	}
}

int handle_request(Connection& connection) {
	int	requestType;
	std::string	location;

	location = connection.getServer().getLocation();
	std::cout << "1\n";
	if (location == "/cgi" || location == "/cgi/") {
		if (connection.getRequest().getMethod() == "DELETE") {
			std::cout << "1\n";

			error_response(connection, METHOD_NOT_ALLOWED);
			return -1;
		}
		if (path_merge_cgi(connection) == -1) {
			std::cout << "1\n";

			return -1;
		}
	} else {
		path_merge_non_cgi(connection);
		std::cout << "1\n";

		if (parse_request_type(connection) == -1)
			return -1;
	}
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	if (requestType == CGI)
		return CGI_handler(connection);
	if (requestType == Directory)
		return directory_handler(connection);
	if (requestType == File)
		return file_handler(connection);
	return -1;
}