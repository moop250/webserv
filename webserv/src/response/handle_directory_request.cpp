/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/03 19:07:45 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

// stuffs to do
// Blocking
int case_index(Connection& connection, std::string& index) {
	int			fd;
	char		buffer[4096];
	long		n;
	std::string	body;
	
	fd = open(index.c_str(), O_RDONLY);
	if (fd < 0) {
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
	while (true) {
		// Blocking here
		n = read(fd, buffer, sizeof(buffer));
		if (n > 0) {
			body.append(buffer, n);
		}
		if (n == 0) {
			break;
		}
		if (n < 0) {
			close(fd);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
	}
	connection.getResponse().setBody(body);
	connection.getResponse().setCode(200);
	connection.getResponse().setCodeMessage("OK");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	std::cout << connection.getResponse() << std::endl;
	return 0;
}

int case_autoindex(Connection& connection) {
	std::string		path;
	DIR				*dir;
	struct dirent	*ent;
	std::string		list;
	std::string		buffer;

	path = connection.getRequest().getPath();
	if (path[0] == '/')
		path.erase(0, 1);
	dir = opendir(path.c_str());
	if (dir == NULL) {
		if (errno == EACCES)
			error_response(connection, FORBIDDEN);
		else
			error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	while (true) {
		ent = readdir(dir);
		if (ent == NULL)
			break;
		std::string file_name(ent->d_name);
		if (file_name == "." || file_name == "..")
			continue;
		list = list + "      <li><a href="
					+ path
					+ "/"
					+ file_name
					+ ">"
					+ file_name
					+ "</a></li>\n";
	}
	list.erase(list.size() - 1);
	closedir(dir);
	buffer = "<!DOCTYPE html>\n"
			"\n"
			"<html lang=\"fr\">\n"
			"  <head>\n"
			"    <meta charset=\"UTF-8\" />\n"
			"  </head>\n"
			"  <body>\n"
			"    <ul>\n"
			+ list + "\n"
			"    </ul>\n"
			"  </body>\n"
			"</html>\n";
	connection.getResponse().setBody(buffer);
	connection.getResponse().setCode(200);
	connection.getResponse().setCodeMessage("OK");
	connection.getResponse().setHeader("Content-Length", size_to_string(buffer.size()));
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

// Untested for autoindex case because config is wonky
// index specified -> return index.html of that location/server
// auto-index on -> return the list of files in that directory in html
// both auto-index off and no index specified -> 403 Fobidden
int get_directory(Connection& connection) {
	std::string		index;
	bool			autoindex;

	index = connection.getServer().index();
	autoindex = connection.getServer().autoindex();
	if (index.empty() && autoindex == false) {
		error_response(connection, FORBIDDEN);
		return -1;
	}
	if (!index.empty()) {
		
		// To test only
		// fix later to replace with absolute path
		// index = ".." + index;
		if (index[0] == '/')
			index.erase(0, 1);

		if (case_index(connection, index) == 0)
			return 0;
	}
	if (autoindex == true) {
		return case_autoindex(connection);
	}
	error_response(connection, NOT_FOUND);
	return -1;
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
// stuffs to do
// Blocking
int post_directory(Connection& connection) {
	std::string		file_name;
	std::string		path;
	std::string		extension;
	int				fd;
	size_t			total;
	long			written;
	std::string		body;

	if (connection.getServer().storage().empty())
		path = connection.getRequest().getPath();
	else
		path = connection.getServer().storage();
	if (path[0] == '/')
		path.erase(0, 1);
	extension = getExtension(connection.getRequest().getContentType());
	while (true) {
		file_name = generate_name(extension);
		if (access(file_name.c_str(), F_OK) != -1)
			continue ;
		if (path[path.size() - 1] != '/')
			path += '/';
		path += file_name;
		break ;
	}
	fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
	if (fd < 0) {
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
	total = 0;
	body = connection.getRequest().getBody();
	while (total < body.size()) {
		// Blocking here
		written = write(fd, body.c_str() + total, body.size() - total);
		if (written < 0) {
			close(fd);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
		if (written == 0)
			break;
		total += written;
	}
	close(fd);
	connection.getResponse().setCode(201);
	connection.getResponse().setCodeMessage("Created");
	
	// Check later to return URI path instead of system path
	connection.getResponse().setHeader("Location", path);
	
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	std::cout << connection.getResponse() << std::endl;
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