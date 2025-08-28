/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file_request.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:01:34 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:44:59 by hoannguy         ###   ########.fr       */
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

int get_file(Connection& connection) {
	std::string		path;
	std::fstream	file;
	std::string		buffer;
	size_t			size;

	path = connection.getRequest().getPath();
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		error_response(connection, INTERNAL_ERROR);
		return -1;	
	}
	connection.getResponse().setContentType(getMIMEType(connection.getRequest().getFileType()));
	file.seekg(0, std::ios::end);
	size = static_cast<size_t>(file.tellg());
	connection.getResponse().setContentLength(size);
	file.seekg(0, std::ios::beg);
	buffer.resize(size);
	// Probably will be blocking for huge file but meh whatever
	if(!file.read(&buffer[0],size)) {
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	file.close();
	connection.getResponse().setBody(buffer);
	connection.getResponse().setCode(200);
	connection.getResponse().setCodeMessage("OK");
	connection.getResponse().setHeader("Content-Length", size_to_string(size));
	connection.getResponse().setHeader("Content-Type", connection.getResponse().getContentType());
	connection.getResponse().constructResponse();
	std::cout << connection.getResponse() << std::endl;
	return 0;
}

int post_file(Connection& connection) {
	(void)connection;

	return -1;
}

int delete_file(Connection& connection) {
	int			code;
	std::string	path;

	path = connection.getRequest().getPath();
	code = std::remove(path.c_str());
	if (code != 0) {
		error_response(connection, INTERNAL_ERROR);
		return -1;	
	}
	connection.getResponse().setCode(204);
	connection.getResponse().setCodeMessage("No Content");
	connection.getResponse().constructResponse();
	std::cout << connection.getResponse() << std::endl;
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