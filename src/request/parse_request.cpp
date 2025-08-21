/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 21:34:14 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

// int parse_headers(Request& request, std::string& buffer) {
// 	std::string::size_type	end_pos;
// 	std::string::size_type	colon_pos;
	
// 	while (true) {
// 		end_pos = buffer.find("\r\n");
// 		if (end_pos == std::string::npos || end_pos == 0)
// 			break;
// 		std::string key_value = buffer.substr(0, end_pos);
// 		colon_pos = key_value.find(":");
// 		if (colon_pos == std::string::npos)
// 			return BAD_REQUEST;
// 		std::string key = key_value.substr(0, colon_pos);
// 		if (key_value[colon_pos + 1] == ' ')
// 			colon_pos += 2;
// 		std::string value = key_value.substr(colon_pos, key_value.size() - 1);
// 		request.setHeader(key, value);
// 		buffer.erase(0, end_pos + 2);
// 	}
// 	std::string contentLength = request.getHeader("Content-Length");
// 	if (!contentLength.empty()) {
// 		size_t len = static_cast<size_t>(strtol(contentLength.c_str(), NULL, 10));
// 		if (len == LONG_MAX) // Compare with server limit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 			return CONTENT_TOO_LARGE;
// 		request.setContentLength(len);
// 		std::string type = request.getHeader("Content-Type");
// 		if (type != "") {
// 			request.setContentType(type);
// 		} else {
// 			request.setContentType("application/octet-stream");
// 		}
// 	} else {
// 		if (request.getMethod() == "POST")
// 			return LENGTH_REQUIRED;
// 	}
// 	return 0;
// }

// int parse_body(Request& request, std::string& buffer) {
// 	size_t	len;

// 	len = request.getContentLength();
// 	buffer.erase(0, 2);
// 	if (static_cast<size_t>(buffer.size()) < len)
// 		return BAD_REQUEST;
// 	try {
// 		std::string body = buffer.substr(0, len);
// 		request.setBody(body);
// 		buffer.erase(0, len);
// 	} catch (std::out_of_range& e) {
// 			return BAD_REQUEST;
// 	}
// 	return 0;
// }

// void parse_cgiType(Request& request) {
// 	std::string path = request.getPath();
// 	std::string extension = path.substr(path.rfind('.'));
// 	if (isCGI(extension) == true) {
// 		request.setRequestType(CGI);
// 		request.setCgiType(extension);
// 	} else
// 		request.setRequestType(File);
// }

// int parse_request_type(Request& request) {
// 	struct stat	fileStat;

// 	if (stat(request.getPath().c_str(), &fileStat) == -1)
// 		return INTERNAL_ERROR;
// 	if (S_ISDIR(fileStat.st_mode)) {
// 		request.setRequestType(Directory);
// 	} else if (S_ISREG(fileStat.st_mode)) {
// 		parse_cgiType(request);
// 	} else
// 		return BAD_REQUEST;
// 	return 0;
// }

int parse_http_ver(Connection& connection) {
	std::string::size_type	http_pos;

	http_pos = connection.buffer.find("\r\n");
	if (http_pos == std::string::npos)
		return READING_HTTPVERSION;
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

	(void)config;
	url_pos = connection.buffer.find(" ");
	if (url_pos == std::string::npos)
		return READING_PATH;
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
		connection.request.setPath(url); // Check if path is redirected before set!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
		return READING_METHOD;
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
	int		code = 0;
	
	switch (connection.state) {
		case READING_METHOD:
			code = parse_method(connection);
			if (code == READING_METHOD || code == READING_PATH)
				return code;
			if (code == NOT_IMPLEMENTED || code == BAD_REQUEST) {
				error_response(code, fd_client);
				return -1;
			}
			break;
		case READING_PATH:
			code = parse_URL(connection, config);
			if (code == READING_PATH || code == READING_HTTPVERSION)
				return code;
			if (code == BAD_REQUEST) {
				error_response(code, fd_client);
				return -1;
			}
			break;
		case READING_HTTPVERSION:
			code = parse_http_ver(connection);
			if (code == READING_HTTPVERSION || code == READING_HEADERS)
				return code;
			if (code == BAD_REQUEST || HTTP_VERSION_MISMATCH) {
				error_response(code, fd_client);
				return -1;
			}
		// case READING_HEADERS:
		// 	code = parse_headers(connection, config);
			
	}

	return 0;
}