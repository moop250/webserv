/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/03 22:20:52 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"
#include "Connection.hpp"

// stuffs to do
// get error page for 301 and 302 then content-length
void error_redirect(Connection& connection, int code) {
	std::string	body;
	std::string	codeMessage;

	code = 302; // temp
	codeMessage = "FOUND"; // temp
	body = "Get the html from error page later"; // temp
	connection.getResponse().setHeader("Location", connection.getRequest().getRedirect());
	connection.getResponse().setCode(code); 
	connection.getResponse().setCodeMessage(codeMessage);
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setBody(body);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
}


// stuffs to do
void error_response(Connection& connection, int code) {
	std::string	body;
	std::string	codeMessage;

	code = 403; // temp
	codeMessage = "FORBIDDEN"; // temp
	body = "Get the html from error page later"; // temp
	connection.getResponse().setCode(code); 
	connection.getResponse().setCodeMessage(codeMessage);
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setBody(body);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	// std::cout << connection.getResponse() << std::endl;
}