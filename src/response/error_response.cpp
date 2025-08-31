/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/29 22:04:30 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"
#include "Connection.hpp"

// stuffs to do
void error_response(Connection& connection, int code) {
	(void)code;
	connection.setState(SENDING_RESPONSE);
}