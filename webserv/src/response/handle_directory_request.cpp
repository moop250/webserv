/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/09 21:06:36 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "GenFD.hpp"
#include "request_handler.hpp"


int case_index(int originFD, t_fdInfo *fdInfo, Connection& connection, std::string& index) {
	int			fd;
	
	index = connection.getRequest().getPath() + index;
	fd = open(index.c_str(), O_RDONLY);
	if (fd < 0) {
		return -1;
	}
	addToGenFD(fdInfo, fd, originFD, SYS_FD_IN);
	connection.setState(CONNECTION_LOCK);
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
		return -1;
	}
	if (path[path.size() - 1] != '/')
		path += "/";
	while (true) {
		ent = readdir(dir);
		if (ent == NULL)
			break;
		std::string file_name(ent->d_name);
		if (file_name == "." || file_name == "..")
			continue;
		list = list + "      <li><a href="
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
int get_directory(int originFD, t_fdInfo *fdInfo, Connection& connection) {
	std::string		index;
	bool			autoindex;

	index = connection.getServer().index();
	autoindex = connection.getServer().autoindex();
	if (index.empty() && autoindex == false) {
		error_response(connection, FORBIDDEN);
		return -1;
	}
	if (!index.empty()) {
		if (index[0] == '/')
			index.erase(0, 1);
		if (case_index(originFD, fdInfo, connection, index) == 0)
			return 0;
	}
	if (autoindex == true) {
		if (case_autoindex(connection) == 0)
			return 0;
	}
	error_response(connection, FORBIDDEN);
	return -1;
}

std::string parseMultiPartForm(Connection& connection) {
	std::string::size_type	boundary_pos;
	std::string::size_type	boundary_end_pos;
	std::string::size_type	header_pos;
	std::string				boundary;
	std::string				boundary_end;
	std::string				body;
	std::string				parsed_body;
	std::string				content_type;
	std::string::size_type	file_name_pos;
	std::string				file_name;
	std::string::size_type	last_slash;

	content_type = connection.getRequest().getContentType();
	if (content_type.find("multipart/form-data") == std::string::npos) {
		error_response(connection, UNSUPPORTED_MEDIA_TYPE);
		return "";
	}
	boundary_pos = content_type.find("boundary=");
	if (boundary_pos == std::string::npos) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	boundary = content_type.substr(boundary_pos + 9);
	if (boundary.empty()) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	boundary = "--" + boundary;
	boundary_end = boundary + "--";
	body = connection.getRequest().getBody();
	if (body.empty()) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	file_name_pos = body.find("filename=");
	if (file_name_pos == std::string::npos) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	file_name_pos += 10;
	while (file_name_pos < body.size() && body[file_name_pos] != '"') {
		file_name.push_back(body[file_name_pos]);
		file_name_pos++;
	}
	last_slash = file_name.find_last_of("/\\");
	if (last_slash != std::string::npos) {
		file_name = file_name.substr(last_slash + 1);
	}
	header_pos = body.find("\r\n\r\n");
	if (header_pos == std::string::npos) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	boundary_end_pos = body.find(boundary_end);
	if (boundary_end_pos == std::string::npos) {
		error_response(connection, BAD_REQUEST);
		return "";
	}
	header_pos += 4;
	if (boundary_end_pos >= 2 && body[boundary_end_pos - 2] == '\r' && body[boundary_end_pos - 1] == '\n') {
		boundary_end_pos -= 2;
	}
	parsed_body = body.substr(header_pos, boundary_end_pos - header_pos);
	connection.getRequest().setBody(parsed_body);
	return file_name;
}

// Upload a file with request body as content.
// stuffs to do
// Blocking
int post_directory(int originFD, t_fdInfo *fdInfo, Connection& connection) {
	std::string		file_name;
	std::string		path;
	std::string		extension;
	std::string		root;
	int				fd;
	// size_t			total;
	// long			written;
	std::string		body;
	std::string		response_body;

	if (connection.getServer().storage().empty())
		path = connection.getRequest().getPath();
	else {
		root = connection.getServer().root();
		if (root[root.size() - 1] == '/')
			root.erase(root.size() - 1);
		path = connection.getServer().storage();
		path = root + path;
	}
	if (path[0] == '/')
		path.erase(0, 1);
	
	file_name = parseMultiPartForm(connection);
	if (file_name.empty())
		return -1;

	if (path[path.size() - 1] != '/')
		path += '/';
	path += file_name;
	fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	addToGenFD(fdInfo, fd, originFD, SYS_FD_IN);
	return 0;
	// add fd to pollfd
	
	// Move this to poll
/* 	total = 0;
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
	// std::cout << connection.getResponse() << std::endl; */
	return 0;  
}

// Reject with 403 Fobidden if Delete request targets directory
int delete_directory(Connection& connection) {
	error_response(connection, FORBIDDEN);
	return -1;
}

int directory_handler(int originFD, t_fdInfo *fdInfo, Connection& connection) {
	std::string	method;

	method = connection.getRequest().getMethod();
	if (method == "GET")
		return get_directory(originFD, fdInfo, connection);
	else if (method == "POST")
		return post_directory(originFD, fdInfo, connection);
	else if (method == "DELETE")
		return delete_directory(connection);
	return -1;
}