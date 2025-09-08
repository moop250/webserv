/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_CGI_request.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:29:51 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/07 12:29:51 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"

int CGI_handler(Connection& connection, char** env) {
	(void)connection;
	(void)env;
	// signal
	// check method with connection.getRequest().getMethod()
	// check cgi type connection.getRequest().getCgiType()
	// fork
	// pipe
	// execve
	// get result to connection.getResponse().setBody()
	return 0;
}