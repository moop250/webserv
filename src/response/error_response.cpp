/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/05 12:35:56 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"
#include "Connection.hpp"

std::string error_message(int code) {
	switch (code) {
		case 301:
			return "Moved Permanently";
		case 302:
			return "Found";
		case 400:
			return "Bad Request";
		case 403:
			return "Forbidden";
		case 404:
			return "Not Found";
		case 405:
			return "Method Not Allowed";
		case 411:
			return "Length Required";
		case 413:
			return "Content Too Large";
		case 415:
			return "Unsupported Media Type";
		case 500:
			return "Internal Error";
		case 501:
			return "Not Implemented";
		case 505:
			return "Http Version Mismatch";
	}
	return "";
}

void error_response(Connection& connection, int code) {
	std::string	body;
	std::string	codeMessage;

	codeMessage = error_message(code);
	if (codeMessage.empty()) {
		code = 500;
		codeMessage = "Internal Error";
	}
	body = connection.getServer().errorPages().content(code);
	if (code == 301 || code == 302) {
		// Check later to return URI path instead of system path
		connection.getResponse().setHeader("Location", connection.getRequest().getRedirect());
	}
	connection.getResponse().setCode(code); 
	connection.getResponse().setCodeMessage(codeMessage);
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setBody(body);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
}