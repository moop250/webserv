/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 18:15:44 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "Response.hpp"



int error_response(int code, int fd_client) {
	Response	response;

	(void)fd_client;
	
	switch (code) {
		case BAD_REQUEST:
			return 400; // Bad Request
		case NOT_FOUND:
			return 404; // Not Found
		case METHOD_NOT_ALLOWED:
			return 405; // Method Not Allowed
		case LENGTH_REQUIRED:
			return 411; // Length Required
		case CONTENT_TOO_LARGE:
			return 413; // Content Too Large
		case NOT_IMPLEMENTED:
			return 501; // Not Implemented
		case HTTP_VERSION_MISMATCH:
			return 505; // HTTP Version Not Supported
		default:
			return 500; // Internal Server Error
	}
}