/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 18:19:21 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

// std::string extract_URL(std::string& url, std::string::size_type query_pos) {
// 	std::string query;

// 	try {
// 		query = url.substr(query_pos + 1, url.size() - 1);
// 		return query;
// 	} catch (std::out_of_range& e) {
// 		return "";
// 	}
// }

// int parse_URL(Request& request, std::string& buffer) {
// 	std::string::size_type	url_pos;
// 	std::string::size_type	path_pos;
// 	std::string::size_type	query_pos;

// 	url_pos = buffer.find(" ");
// 	if (url_pos == std::string::npos)
// 		return BAD_REQUEST;
// 	std::string url = buffer.substr(0, url_pos);
// 	if (url[0] == '/' || !url.compare(0, 7, "http://") || !url.compare(0, 8, "https://")) {
// 		if (!url.compare(0, 7, "http://")) {
// 			path_pos = url.find("/", 8);
// 			if (path_pos == std::string::npos)
// 				return BAD_REQUEST;
// 			url.erase(0, path_pos);
// 		}
// 		else if (!url.compare(0, 8, "https://")) {
// 			path_pos = url.find("/", 9);
// 			if (path_pos == std::string::npos)
// 				return BAD_REQUEST;
// 			url.erase(0, path_pos);
// 		}
// 		query_pos = url.find("?");
// 		if (query_pos != std::string::npos) {
// 			std::string query = extract_URL(url, query_pos);
// 			if (query == "") {
// 				return BAD_REQUEST;
// 			}
// 			request.setQuery(query);
// 			url.erase(query_pos, url_pos);
// 		}
// 		request.setPath(url); // Check if path is redirected before set!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 		buffer.erase(0, url_pos + 1);
// 		return 0;
// 	}
// 	return BAD_REQUEST;
// }

// int parse_http_ver(Request& request, std::string& buffer) {
// 	std::string::size_type	http_pos;

// 	http_pos = buffer.find("\r\n");
// 	if (http_pos == std::string::npos)
// 		return BAD_REQUEST;
// 	std::string http_ver = buffer.substr(0, http_pos);
// 	if (http_ver == "HTTP/1.0" || http_ver == "HTTP/2.0" || http_ver == "HTTP/3.0")
// 		return HTTP_VERSION_MISMATCH;
// 	else if (http_ver == "HTTP/1.1") {
// 		request.setHttpVersion(http_ver);
// 		buffer.erase(0, http_pos + 2);
// 		return 0;
// 	}
// 	return BAD_REQUEST;
// }

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
	}

	return 0;
}