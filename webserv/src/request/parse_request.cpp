/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/07 19:07:21 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

int parse_body(Connection& connection) {
	size_t		len;
	std::string	body;

	len = connection.getRequest().getContentLength();
	if (static_cast<size_t>(connection.buffer.size()) < len)
		return CONTINUE_READ;
	body = connection.buffer.substr(0, len);
	connection.getRequest().setBody(body);
	connection.buffer.erase(0, len); 
	connection.setState(MAKING_RESPONSE);
	return MAKING_RESPONSE;
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
		connection.getRequest().setHttpVersion(http_ver);
		connection.buffer.erase(0, http_pos + 2);
		connection.setState(READING_HEADERS);
		return READING_HEADERS;
	}
	return BAD_REQUEST;
}

std::string extract_Query(std::string& url, std::string::size_type query_pos) {
	std::string query;

	try {
		query = url.substr(query_pos + 1, url.size() - 1);
		return query;
	} catch (std::out_of_range& e) {
		return "";
	}
}

int parse_URL(Connection& connection) {
	std::string::size_type	url_pos;
	std::string::size_type	path_pos;
	std::string::size_type	query_pos;

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
			std::string query = extract_Query(url, query_pos);
			connection.getRequest().setQuery(query);
			url.erase(query_pos, url_pos);
		}
		connection.getRequest().setPath(url);
		connection.buffer.erase(0, url_pos + 1);
		connection.setState(READING_HTTPVERSION);
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
		connection.getRequest().setMethod(method);
		connection.buffer.erase(0, position + 1);
		connection.setState(READING_PATH);
		return READING_PATH;
	} else if (method == "HEAD" || method == "PUT"
				|| method == "CONNECT" || method == "OPTIONS"
				|| method == "TRACE" || method == "PATCH")
		return NOT_IMPLEMENTED;
	return BAD_REQUEST;
}

int parse_request(Connection& connection, Config& config) {
	int		code;

	code = -1;
	switch (static_cast<int>(connection.getState())) {
		case READING_METHOD:
			code = parse_method(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_PATH:
					return parse_request(connection, config);
				case NOT_IMPLEMENTED:
					// fall through
				case BAD_REQUEST:
					connection.setClose(true);
					error_response(connection, code);
					return -1;
			}
			break ;
			
		case READING_PATH:
			code = parse_URL(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_HTTPVERSION:
					return parse_request(connection, config);
				case INTERNAL_ERROR:
					// fall through
				case BAD_REQUEST:
					connection.setClose(true);
					error_response(connection, code);
					return -1;
			}
			break ;
			
		case READING_HTTPVERSION:
			code = parse_http_ver(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_HEADERS:
					return parse_request(connection, config);
				case HTTP_VERSION_MISMATCH:
					// fall through
				case BAD_REQUEST:
					connection.setClose(true);
					error_response(connection, code);
					return -1;
			}
			break ;
			
		case READING_HEADERS:
			code = parse_headers(connection, config);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case READING_BODY:
					return parse_request(connection, config);
				case READING_CHUNKED:
					return parse_body_chunked(connection);
				case MAKING_RESPONSE:
					return MAKING_RESPONSE;
				case MOVED_PERMANENTLY:
					// fall through
				case FOUND:
					error_response(connection, code);
					return -1;
				case FORBIDDEN:
					// fall through
				case METHOD_NOT_ALLOWED:
					// fall through
				case LENGTH_REQUIRED:
					// fall through
				case INTERNAL_ERROR:
					// fall through
				case BAD_REQUEST:
					connection.setClose(true);
					error_response(connection, code);
					return -1;
			}
			break ;

		case READING_CHUNKED:
			code = parse_body_chunked(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case MAKING_RESPONSE:
					return MAKING_RESPONSE;
				case BAD_REQUEST:
					// fall through
				case CONTENT_TOO_LARGE:
					connection.setClose(true);
					error_response(connection, code);
					return -1;
			}
			break ;

		case READING_BODY:
			code = parse_body(connection);
			switch (code) {
				case CONTINUE_READ:
					return CONTINUE_READ;
				case MAKING_RESPONSE:
					return MAKING_RESPONSE;
			}
			break ;
	}
	return code;
}