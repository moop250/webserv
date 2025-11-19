/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/19 09:52:27 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

// MOOP -> case index remake
int case_index_remake(Connection& connection) {
	int			fdin;
	char		buffer[FILE_CHUNK_SIZE];
	long		n;
	int			size;
	
	fdin = connection.getFDIN();
	if (connection.getState() == IO_OPERATION) {
		n = read(fdin, buffer, sizeof(buffer));
		if (n > 0) {
			connection.getResponse().appendBody(buffer, n);
			return 0;
		}
		if (n == 0) {
			close(fdin);
			connection.setFDIN(-1);
			connection.setState(MAKING_RESPONSE);
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
		connection.getResponse().setHeader("Content-Type", "text/html");
		if (connection.getRequest().getKeepAlive() == "keep-alive")
			connection.getResponse().setHeader("Connection", "keep-alive");
		connection.getResponse().constructResponse();
		connection.setState(SENDING_RESPONSE);
		// std::cout << connection.getResponse() << std::endl;
	}
	return 0;
}

int case_autoindex(Connection& connection) {
	std::string		path;
	DIR				*dir;
	struct dirent	*ent;
	std::string		list;
	std::string		buffer;
	std::string		base_path;
	std::string		location;

	base_path = connection.getRequest().getBasePath();
	if (base_path[0] == '/')
		base_path.erase(0, 1);
	if (base_path[base_path.size() - 1] != '/')
		base_path += "/";
	location = connection.getServer().getLocation();
	base_path.erase(0, location.size() - 1);
	path = connection.getRequest().getPath();
	if (path[0] == '/')
		path.erase(0, 1);
	if (path[path.size() - 1] != '/')
		path += "/";
	dir = opendir(path.c_str());
	if (dir == NULL) {
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
					+ base_path
					+ file_name
					+ ">"
					+ file_name
					+ "</a></li>\n";
	}
	if (!list.empty())
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
	if (connection.getRequest().getKeepAlive() == "keep-alive")
		connection.getResponse().setHeader("Connection", "keep-alive");
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
	return 0;
}

// index specified -> return index.html of that location/server
// auto-index on -> return the list of files in that directory in html
// both auto-index off and no index specified or error -> 403 Fobidden
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
		if (case_index_remake(connection) == 0)
			return 0;
	}
	if (autoindex == true) {
		if (case_autoindex(connection) == 0)
			return 0;
	}
	error_response(connection, FORBIDDEN);
	return -1;
}

// MOOP -> post directory remake
int post_directory_remake(Connection& connection) {
	std::string		extension;
	int				fdout;
	long			written;
	std::string		body;
	size_t			to_write;
	std::string		response_body;

	body = connection.getRequest().getBody();
	fdout = connection.getFDOUT();
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
		connection.getResponse().setCode(200);
		connection.getResponse().setCodeMessage("OK");
		if (connection.getRequest().getKeepAlive() == "keep-alive")
			connection.getResponse().setHeader("Connection", "keep-alive");
		connection.getResponse().setHeader("Content-Type", "text/html");
		response_body = "<!doctype html>\n\n<html><body><h1>Upload successful!</h1><p>Your file has been received.</p></body></html>";
		connection.getResponse().setHeader("Content-Length", size_to_string(response_body.size()));
		connection.getResponse().setBody(response_body);
		connection.getResponse().constructResponse();
		connection.setState(SENDING_RESPONSE);
		// std::cout << connection.getResponse() << std::endl;
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
		return post_directory_remake(connection);
	else if (method == "DELETE")
		return delete_directory(connection);
	return -1;
}