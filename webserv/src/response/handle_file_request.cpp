/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file_request.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:01:34 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/03 19:08:11 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

std::string size_to_string(size_t size) {
	std::ostringstream	oss;
	std::string			str;

    oss << size;
    str = oss.str();
	return str;
}

// return the file
int get_file(Connection& connection) {
	std::string	path;
	std::string	body;
	int			fd;
	char		buffer[4096];
	long		n;

	path = connection.getRequest().getPath();

	fd = open(path.c_str(), O_RDONLY);
	if (fd < 0) {
		switch (errno) {
			case EACCES:
				error_response(connection, FORBIDDEN);
				break ;
			default:
				error_response(connection, INTERNAL_ERROR);
				break ;
		}
		return -1;
	}
	connection.getResponse().setContentType(getMIMEType(connection.getRequest().getFileType()));
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
	connection.getResponse().setContentLength(body.size());
	connection.getResponse().setBody(body);
	connection.getResponse().setCode(200);
	connection.getResponse().setCodeMessage("OK");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setHeader("Content-Type", connection.getResponse().getContentType());
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

// Append the request body to the file if enough permission
// Stuffs to do
int post_file(Connection& connection) {
	std::string		path;
	int				fd;
	size_t			total;
	long			written;
	std::string		body;

	path = connection.getRequest().getPath();

	// remove later
	if (path[0] == '/')
		path.erase(0, 1);
	
	fd = open(path.c_str(), O_WRONLY | O_APPEND, 0644);
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
	// extension validation so eg: not append jpeg into html?
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
	connection.getResponse().setCode(204);
	connection.getResponse().setCodeMessage("No Content");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

// Just delete the file if have permission
int delete_file(Connection& connection) {
	std::string	path;
	std::string parent_directory;

	path = connection.getRequest().getPath();
	if (path[0] == '/')
		path.erase(0, 1);
	if (std::remove(path.c_str()) != 0) {
		switch (errno) {
			case EACCES:
				// fallthrough
			case EPERM:
				error_response(connection, FORBIDDEN);
				break;
			case ENOENT:
				error_response(connection, NOT_FOUND);
				break;
			default:
				error_response(connection, INTERNAL_ERROR);
				break;
		}
		return -1;
	}
	connection.getResponse().setCode(204);
	connection.getResponse().setCodeMessage("No Content");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

int file_handler(Connection& connection) {
	std::string	method;

	method = connection.getRequest().getMethod();
	if (method == "GET")
		return get_file(connection);
	else if (method == "POST")
		return post_file(connection);
	else if (method == "DELETE")
		return delete_file(connection);
	return -1;
}