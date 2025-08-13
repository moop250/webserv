/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 23:20:23 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"

int parse_method(Request& request, std::string& buffer) {
	std::string::size_type	position;

	position = buffer.find(" ");
	if (position == std::string::npos)
		return 400;
	try {
		std::string method = buffer.substr(0, position);
		if (method == "GET" || method == "POST" || method == "DELETE") {
			request.setMethod(method);
			buffer.erase(0, position + 1);
			return 0;
		} else if (method == "HEAD" || method == "PUT"
					|| method == "CONNECT" || method == "OPTIONS"
					|| method == "TRACE" || method == "PATCH")
			return 501;
		return 400;
	} catch (std::out_of_range& e) {
		return 400;
	}
	return 0;
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

int parse_URL(Request& request, std::string& buffer) {
	std::string::size_type	url_pos;
	std::string::size_type	path_pos;
	std::string::size_type	query_pos;

	url_pos = buffer.find(" ");
	if (url_pos == std::string::npos)
		return 400;
	try {
		std::string url = buffer.substr(0, url_pos);
		if (url[0] == '/' || !url.compare(0, 7, "http://") || !url.compare(0, 8, "https://")) {
			if (!url.compare(0, 7, "http://")) {
				path_pos = url.find("/", 8);
				if (path_pos == std::string::npos)
					return 400;
				url.erase(0, path_pos);
			}
			else if (!url.compare(0, 8, "https://")) {
				path_pos = url.find("/", 9);
				if (path_pos == std::string::npos)
					return 400;
				url.erase(0, path_pos);
			}
			query_pos = url.find("?");
			if (query_pos != std::string::npos) {
				std::string query = extract_URL(url, query_pos);
				if (query == "") {
					return 400;
				}
				request.setQuery(query);
				url.erase(query_pos, url_pos);
			}
			request.setPath(url);
			buffer.erase(0, url_pos + 1);
			return 0;
		}
		return 400;
	} catch (std::out_of_range& e) {
		return 400;
	}
	return 0;
}

int parse_http_ver(Request& request, std::string& buffer) {
	std::string::size_type	http_pos;

	http_pos = buffer.find("\r\n");
	if (http_pos == std::string::npos)
		return 400;
	try {
		std::string http_ver = buffer.substr(0, http_pos);
		if (http_ver == "HTTP/1.0" || http_ver == "HTTP/2.0" || http_ver == "HTTP/3.0")
			return 505;
		else if (http_ver == "HTTP/1.1") {
			request.setHttpVersion(http_ver);
			buffer.erase(0, http_pos + 2);
			return 0;
		}
		return 400;
	} catch (std::out_of_range& e) {
		return 400;
	}
	return 0;
}

int parse_request(std::string& buffer) {
	Request	request;
	int		code = 0;
	
	if (buffer.empty())
		return error_response(400);
	code = parse_method(request, buffer);
	if (code != 0)
		return error_response(code);
	code = parse_URL(request, buffer);
	if (code != 0)
		return error_response(code);
	code = parse_http_ver(request, buffer);
	if (code != 0)
		return error_response(code);
	if (buffer == "\r\n")
		return handle_request(request);
	return 0;
}