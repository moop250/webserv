/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file_request.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:01:34 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/12 09:39:29 by hoannguy         ###   ########.fr       */
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

// Just delete the file if have permission
int delete_file(Connection& connection) {
	std::string	path;
	std::string parent_directory;

	std::cout << "Deleting file..." << std::endl;
	path = connection.getRequest().getPath();
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
	if (connection.getRequest().getKeepAlive() == "keep-alive")
		connection.getResponse().setHeader("Connection", "keep-alive");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

// MOOP -> new post file
int post_file_remake(Connection& connection) {
	int				fdout;
	long			written;
	std::string		body;
	size_t			to_write;

	body = connection.getRequest().getBody();
	fdout = connection.getFDOUT();
	if (body.substr(0, 8) == "content=") {
		body.erase(0, 8);
		connection.getRequest().removeBody(0, 8);
	}
	if (connection.getState() == IO_OPERATION) {
		to_write = std::min((size_t)FILE_WRITE_SIZE, body.size());
		written = write(fdout, body.c_str(), to_write);
		if (written < 0) {
			close(fdout);
			connection.setFDOUT(-1);
			connection.setOperation(No);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
		if (written > 0) {
			connection.getRequest().removeBody(0, written);
			body.erase(0, written);
			if (!body.empty())
				return 0;
		}
		close(fdout);
		connection.setFDOUT(-1);
		connection.setState(MAKING_RESPONSE);
		connection.setOperation(No);
	}
	if (connection.getState() == MAKING_RESPONSE) {
		connection.getResponse().setCode(204);
		connection.getResponse().setCodeMessage("No Content");
		if (connection.getRequest().getKeepAlive() == "keep-alive")
			connection.getResponse().setHeader("Connection", "keep-alive");
		connection.getResponse().constructResponse();
		connection.setState(SENDING_RESPONSE);
		// std::cout << connection.getResponse() << std::endl;
	}
	return 0;
}

// MOOP -> new get file
int get_file_remake(Connection& connection) {
	int			fdin;
	char		buffer[FILE_CHUNK_SIZE];
	long		n;
	int			size;

	connection.getResponse().setContentType(getMIMEType(connection.getRequest().getFileType()));
	fdin = connection.getFDIN();
	// if data left to read
	if (connection.getState() == IO_OPERATION) {
		n = read(fdin, buffer, sizeof(buffer));
		if (n > 0) {
			connection.getResponse().appendBody(buffer, n);
			return 0;
		}
		// No data left
		if (n == 0) {
			close(fdin);
			// set fdin to -1
			connection.setFDIN(-1);
			// state to MAKING_RESPONSE
			connection.setState(MAKING_RESPONSE);
			// operation to No
			connection.setOperation(No);
		}
		if (n < 0) {
			close(fdin);
			connection.setFDIN(-1);
			connection.setOperation(No);
			error_response(connection, INTERNAL_ERROR);
			return -1;
		}
	}
	if (connection.getState() == MAKING_RESPONSE) {
		size = connection.getResponse().getBody().size();
		connection.getResponse().setContentLength(size);
		connection.getResponse().setCode(200);
		connection.getResponse().setCodeMessage("OK");
		connection.getResponse().setHeader("Content-Length", size_to_string(size));
		connection.getResponse().setHeader("Content-Type", connection.getResponse().getContentType());
		if (connection.getRequest().getKeepAlive() == "keep-alive")
			connection.getResponse().setHeader("Connection", "keep-alive");
		connection.getResponse().constructResponse();
		connection.setState(SENDING_RESPONSE);
		// std::cout << connection.getResponse() << std::endl;
	}
	return 0;
}

int file_handler_remake(Connection& connection) {
	std::string	method;

	method = connection.getRequest().getMethod();
	if (method == "GET")
		return get_file_remake(connection);
	else if (method == "POST")
		return post_file_remake(connection);
	else if (method == "DELETE") {
		std::cout << "Handling file request remake..." << std::endl;
		return delete_file(connection);
	}
	return -1;
}
