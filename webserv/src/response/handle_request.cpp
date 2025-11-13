/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:19:26 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/13 18:19:44 by hoannguy         ###   ########.fr       */
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

// MOOP -> open new fd for file
int open_FILE_FD(Connection& connection, std::string& method) {
	std::string	path;

	path = connection.getRequest().getPath();
	if (method == "GET") {
		// set fdin
		connection.setFDIN(open(path.c_str(), O_RDONLY));
		if (connection.getFDIN() < 0) {
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
		// set state to IO_OPERATION
		connection.setState(IO_OPERATION);
		// fdout is -1 by default
		// set operation to in
		connection.setOperation(In);
	} else if (method == "POST") {
		// set fdout
		connection.setFDOUT(open(path.c_str(), O_WRONLY | O_APPEND, 0644));
		if (connection.getFDOUT() < 0) {
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
		// set state to IO_OPERATION
		connection.setState(IO_OPERATION);
		// fdin is -1 by default
		// set operation to in
		connection.setOperation(Out);
	}
	return 0;
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

// MOOP -> open new fd for directory
// not opening any fd for POST here yet, to remake
int open_DIR_FD(Connection& connection, std::string& method) {
	std::string	index;

	if (method == "GET") {
		index = connection.getServer().index();
		index = connection.getRequest().getPath() + index;
		if (!index.empty()) {
			if (index[0] == '/')
				index.erase(0, 1);
			connection.setFDIN(open(index.c_str(), O_RDONLY));
			if (connection.getFDIN() < 0) {
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
			// set state to IO_OPERATION
			connection.setState(IO_OPERATION);
			// fdout is -1 by default
			// set operation to in
			connection.setOperation(In);
		}
	} else if (method == "POST") {
		std::string	file_name;
		std::string	path;
		std::string	root;
		int			fd;

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
		connection.setFDOUT(fd);
		connection.setState(IO_OPERATION);
		connection.setOperation(Out);
	}
	return 0;
}

int open_CGI_PIPE_FORK(Connection& connection, std::string& method) {
	std::vector<std::string>	env_string;
	std::vector<char*>			env_pointer;
	char						**env;
	int							in[2];
	int							out[2];

	set_env(connection, env_string);
	env = build_env(env_string, env_pointer);
	if (pipe(in) < 0) { 
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	if (pipe(out) < 0) {
		close(in[0]);
		close(in[1]);
		error_response(connection, INTERNAL_ERROR);
		return -1;
	}
	connection.setPid(fork());
	switch(connection.getPid()) {
		case -1:
			error_response(connection, INTERNAL_ERROR);
			return -1;
		case 0:
			child_launch_CGI(connection, in, out, env);
			return 0;
		default:
			connection.setCGItime(std::time(NULL));
			close(in[0]);
			close(out[1]);
			connection.setFDOUT(in[1]);
    		connection.setFDIN(out[0]);
			connection.setState(IO_OPERATION);
			if (method == "POST") {
				connection.setOperation(Out);
			} else if (method == "GET") {
				connection.setOperation(In);
			}
			return 0;
	}
	return 0;
}

// MOOP -> Parse_request_type qui vient apres parse_request()
int parse_type_fd(Connection& connection) {
	int			requestType;
	std::string	method;
	std::string	location;

	location = connection.getServer().getLocation();
	if (location == "/cgi" || location == "/cgi/") {
		if (connection.getRequest().getMethod() == "DELETE") {
			error_response(connection, METHOD_NOT_ALLOWED);
			return -1;
		}
		if (path_merge_cgi(connection) == -1) {
			return -1;
		}
	} else {
		path_merge_non_cgi(connection);
		if (parse_request_type(connection) == -1)
			return -1;
	}
	requestType = static_cast<int>(connection.getRequest().getRequestType());
	connection.setRequestType(requestType);
	method = connection.getRequest().getMethod();
	if (requestType == CGI && (method == "GET" || method == "POST")) {
		return open_CGI_PIPE_FORK(connection, method);
	}
	if (requestType == Directory && (method == "GET" || method == "POST" || method == "DELETE")) {
		return open_DIR_FD(connection, method);
	}
	if (requestType == File && (method == "GET" || method == "POST" || method == "DELETE")) {
		return open_FILE_FD(connection, method);
	}
	return 0;
}

// MOOP -> le nouveau handle_request
int handle_request_remake(Connection& connection) {
	int	requestType;

	requestType = connection.getRequestType();
	if (requestType == CGI)
		return CGI_handler_remake(connection);
	if (requestType == Directory)
		return directory_handler(connection);
	if (requestType == File)
		return file_handler_remake(connection);
	return -1;
}