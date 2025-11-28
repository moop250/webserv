/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/28 21:48:56 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"
#include "Connection.hpp"

std::string error_message(std::string& code) {
	if (code == "200")
		return "OK";
	if (code == "201")
		return "Created";
	if (code == "202")
		return "Accepted";
	if (code == "204")
		return "No Content";
	if (code == "301")
		return "Moved Permanently";
	if (code == "302")
		return "Found";
	if (code == "400")
		return "Bad Request";
	if (code == "403")
		return "Forbidden";
	if (code == "404")
		return "Not Found";
	if (code == "405")
		return "Method Not Allowed";
	if (code == "411")
		return "Length Required";
	if (code == "413")
		return "Content Too Large";
	if (code == "415")
		return "Unsupported Media Type";
	if (code == "500")
		return "Internal Server Error";
	if (code == "501")
		return "Not Implemented";
	if (code == "504")
		return "Gateway Timeout";
	if (code == "505")
		return "Http Version Mismatch";
	return "";
}

std::string error_message(int code) {
	switch (code) {
		case 200:
			return "OK";
		case 201:
			return "Created";
		case 202:
			return "Accepted";
		case 204:
			return "No Content";
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
			return "Internal Server Error";
		case 501:
			return "Not Implemented";
		case 504:
			return "Gateway Timeout";
		case 505:
			return "Http Version Mismatch";
	}
	return "";
}

void error_response(Connection& connection, int code) {
	std::string	body;
	std::string	codeMessage;

	codeMessage = error_message(code);
	std::cout << ROSE << "[DEBUG]		: " << RESET << "Error message is " << YELLOW << codeMessage << std::endl;
	if (codeMessage.empty()) {
		code = 500;
		codeMessage = "Internal Server Error";
		body = "<!DOCTYPE html>\n"
				"\n"
				"<html lang=\"fr\">\n"
				"  <head>\n"
				"    <meta charset=\"UTF-8\" />\n"
				"  </head>\n"
				"  <body>\n"
				"    <div style=\"margin-top: 50px; margin-bottom:200px; text-align: center; text-justify: center;\">\n"
				"      <h1 style=\"font-size: 5rem;\">500</h1>\n"
				"      <h3 style=\"font-size: 2rem;\">Internal Server Error</h3>\n"
				"      <br />\n"
				"      <p>Un problème du serveur est survenu!</p>\n"
				"    </div>\n"
				"  </body>\n"
				"</html>\n";
	} else {
		body = connection.getServer().errorPages().content(code);
		if (code == 301 || code == 302) {
			std::string redirect = connection.getRequest().getRedirect();
			connection.getResponse().setHeader("Location", redirect);
		}
	}
	std::string path = connection.getRequest().getPath();
	connection.getResponse().setCode(code); 
	connection.getResponse().setCodeMessage(codeMessage);
	// if (connection.getClose() == true)
	// 	connection.getResponse().setHeader("Connection", "close");
	// else
	// 	connection.getResponse().setHeader("Connection", "keep-alive");
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setBody(body);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
}