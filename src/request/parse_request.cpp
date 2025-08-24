/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/24 14:09:36 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

void parse_type(Connection& connection) {
	std::string path;
	std::string extension;

	path = connection.request.getPath();
	extension = path.substr(path.rfind('.'));
	if (isCGI(extension) == true) {
		connection.request.setRequestType(CGI);
		connection.request.setCgiType(extension);
	} else
		connection.request.setRequestType(File);
}

int parse_request_type(Connection& connection) {
	struct stat	fileStat;
	std::string	path;

	// Check if path is cgi or not, pre-append the cgi path or root path
	path = connection.request.getPath();
	
	if (stat(path.c_str(), &fileStat) == -1)
		return INTERNAL_ERROR;
	if (S_ISDIR(fileStat.st_mode)) {
		connection.request.setRequestType(Directory);
	} else if (S_ISREG(fileStat.st_mode)) {
		parse_type(connection);
	} else
		return BAD_REQUEST;
	return CONTINUE_READ;
}

int parse_body(Connection& connection) {
	size_t		len;
	std::string	body;

	len = connection.request.getContentLength();
	if (static_cast<size_t>(connection.buffer.size()) < len)
		return CONTINUE_READ;
	body = connection.buffer.substr(0, len);
	connection.request.setBody(body);
	connection.buffer.erase(0, len); 
	connection.state = READING_COMPLETE;
	return READING_COMPLETE;
}

int parse_http_ver(Connection& connection) {
	std::string::size_type	http_pos;

	http_pos = connection.buffer.find("\r\n");
	if (http_pos == std::string::npos)
		return CONTINUE_READ;
	std::string http_ver = connection.buffer.substr(0, http_pos);
	if (http_ver == "HTTP/1.0" || http_ver == "HTTP/2.0" || http_ver == "HTTP/3.0")
		return HTTP_VERSION_MISMATCH;
	else if (http_ver == "HTTP/1.1") {
		connection.request.setHttpVersion(http_ver);
		connection.buffer.erase(0, http_pos + 2);
		connection.state = READING_HEADERS;
		return READING_HEADERS;
	}
	return BAD_REQUEST;
}

std::string extract_URL(std::string& url, std::string::size_type query_pos) {
	std::string query;

	try {
		query = url.substr(query_pos + 1, url.size() - 1);
		return query;
	} catch (std::out_of_range& e) {
		return "";
	}
}

int parse_URL(Connection& connection, Config& config) {
	std::string::size_type	url_pos;
	std::string::size_type	path_pos;
	std::string::size_type	query_pos;
	int						error;

	(void)error;
	(void)config;
	url_pos = connection.buffer.find(" ");
	if (url_pos == std::string::npos)
		return CONTINUE_READ;
	std::string url = connection.buffer.substr(0, url_pos);
	if (url[0] == '/' || !url.compare(0, 7, "http://") || !url.compare(0, 8, "https://")) {
		if (!url.compare(0, 7, "http://")) {
			path_pos = url.find("/", 8);
			if (path_pos == std::string::npos)
				return BAD_REQUEST;
			url.erase(0, path_pos);
		}
		else if (!url.compare(0, 8, "https://")) {
			path_pos = url.find("/", 9);
			if (path_pos == std::string::npos)
				return BAD_REQUEST;
			url.erase(0, path_pos);
		}
		query_pos = url.find("?");
		if (query_pos != std::string::npos) {
			std::string query = extract_URL(url, query_pos);
			if (query == "") {
				return BAD_REQUEST;
			}
			connection.request.setQuery(query);
			url.erase(query_pos, url_pos);
		}
		
		 // Check if path is redirected before set!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		connection.request.setPath(url);
		// error = parse_request_type(connection);
		// if (error != CONTINUE_READ)
		// 	return error;
		
		connection.buffer.erase(0, url_pos + 1);
		connection.state = READING_HTTPVERSION;
		return READING_HTTPVERSION;
	}
	return BAD_REQUEST;
}

int parse_method(Connection& connection) {
	std::string::size_type	position;

	position = connection.buffer.find(" ");
	if (position == std::string::npos)
		return CONTINUE_READ;
	std::string method = connection.buffer.substr(0, position);
	if (method == "GET" || method == "POST" || method == "DELETE") {
		connection.request.setMethod(method);
		connection.buffer.erase(0, position + 1);
		connection.state = READING_PATH;
		return READING_PATH;
	} else if (method == "HEAD" || method == "PUT"
				|| method == "CONNECT" || method == "OPTIONS"
				|| method == "TRACE" || method == "PATCH")
		return NOT_IMPLEMENTED;
	return BAD_REQUEST;
}

int parse_request(Connection& connection, Config& config, int fd_client, char **env) {
	int		code = -1;
	
	switch (static_cast<int>(connection.state)) {
		case READING_METHOD:
			code = parse_method(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_PATH:
					return parse_request(connection, config, fd_client, env);
				case NOT_IMPLEMENTED:
					// fall through
				case BAD_REQUEST:
					error_response(code, fd_client);
					return -1;
			}
			break ;
			
		case READING_PATH:
			code = parse_URL(connection, config);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_HTTPVERSION:
					return parse_request(connection, config, fd_client, env);
				case INTERNAL_ERROR:
					// fall through
				case BAD_REQUEST:
					error_response(code, fd_client);
					return -1;
			}
			break ;
			
		case READING_HTTPVERSION:
			code = parse_http_ver(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_HEADERS:
					return parse_request(connection, config, fd_client, env);
				case HTTP_VERSION_MISMATCH:
					// fall through
				case BAD_REQUEST:
					error_response(code, fd_client);
					return -1;
			}
			break ;
			
		case READING_HEADERS:
			code = parse_headers(connection, config);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_BODY:
					return parse_request(connection, config, fd_client, env);
				case READING_COMPLETE:
					handle_request(connection, config, fd_client, env);
					return READING_COMPLETE;
				case METHOD_NOT_ALLOWED:
					// fall through
				case LENGTH_REQUIRED:
					error_response(code, fd_client);
					return -1;
			}
			break ;
		case READING_BODY:
			code = parse_body(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_COMPLETE:
					handle_request(connection, config, fd_client, env);
					return READING_COMPLETE;
			}
			break ;
	}
	return code;
}