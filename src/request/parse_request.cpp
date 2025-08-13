/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 18:17:42 by hoannguy         ###   ########.fr       */
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

// std::string extract_URL(std::string& url, std::string::size_type position) {
// 	std::string query;

// 	try {
// 		query = url.substr()
// 	}
// }

int parse_URL(Request& request, std::string& buffer) {
	std::string::size_type	position1;
	std::string::size_type	position2;

	position1 = buffer.find(" ");
	if (position1 == std::string::npos)
		return 400;
	try {
		std::string url = buffer.substr(0, position1);
		if (url[0] == '/' || !url.compare(0, 7, "http://") || !url.compare(0, 8, "https://")) {
			// position2 = url.find("?");
			// if (position2 != std::string::npos) {
			// 	std::string query = extract_URL(url, position2);
			// 	if (query == "") {
			// 		return 400;
			// 	}
			// 	request.setQuery(query);
			// 	url.erase(position2, position1);
			// }
			request.setURL(url);
			buffer.erase(0, position1 + 1);
			return 0;
		}
		return 400;
	} catch (std::out_of_range& e) {
		return 400;
	}
}

int parse_request(std::string& buffer) {
	Request	request;
	int		code = 0;
	
	if (buffer.empty())
		return response(400);
	code = parse_method(request, buffer);
	if (code != 0)
		return response(code);
	code = parse_URL(request, buffer);
	if (code != 0)
		return response(code);
	return 0;
}