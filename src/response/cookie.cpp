/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:57:17 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/28 12:05:39 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "request_handler.hpp"

int login_handler(Connection& connection) {
	std::string	user;
	std::string	cookie;

	user = connection.getRequest().getBody();
	cookie = "user=" + user + "; Path=/; Max-Age=10";
	connection.getResponse().setCode(204);
	connection.getResponse().setCodeMessage("No Content");
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().setHeader("Set-Cookie", cookie);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	return 0;
}

int logout_handler(Connection& connection) {
	std::string	user;
	std::string	cookie;

	user = connection.getRequest().getCookie("user");
	cookie = "user=" + user + "; Path=/; Max-Age=0";
	connection.getResponse().setCode(204);
	connection.getResponse().setCodeMessage("No Content");
	connection.getResponse().setHeader("Connection", "close");
	connection.getResponse().setHeader("Set-Cookie", cookie);
	connection.getResponse().constructResponse();
	connection.setState(SENDING_RESPONSE);
	return 0;
}