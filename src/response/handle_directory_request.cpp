/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_directory_request.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:29 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/29 16:57:19 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"

int get_directory(Connection& connection) {
	(void)connection;
	return 0;
}
int post_directory(Connection& connection) {
	(void)connection;
	return 0;
}
int delete_directory(Connection& connection) {
	(void)connection;
	return 0;
}

int directory_handler(Connection& connection) {
	std::string	method;

	method = connection.getRequest().getMethod();
	if (method == "GET")
		return get_directory(connection);
	else if (method == "POST")
		return post_directory(connection);
	else if (method == "DELETE")
		return delete_directory(connection);
	return -1;
}