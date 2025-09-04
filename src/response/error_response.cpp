/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/03 22:47:20 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"
#include "Connection.hpp"

// stuffs to do
void error_response(Connection& connection, int code) {
	std::string	body;
	std::string	codeMessage;

	codeMessage = "THIS IS TEMP"; // temp
	body = "<!DOCTYPE>\n"
			"\n"
			"<html lang=\"fr\">\n"
			"  <head></head>\n"
			"  <body>\n"
			"      <h1>--- THIS IS A TEMP MESSAGE! ---</h1>\n"
			"  </body>\n"
			"</html>\n"; // temp
	if (code == 301 || code == 302)
		connection.getResponse().setHeader("Location", connection.getRequest().getRedirect());
	connection.getResponse().setCode(code); 
	connection.getResponse().setCodeMessage(codeMessage);
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().setHeader("Content-Type", "text/html");
	connection.getResponse().setHeader("Content-Length", size_to_string(body.size()));
	connection.getResponse().setBody(body);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	std::cout << connection.getResponse() << std::endl;
}