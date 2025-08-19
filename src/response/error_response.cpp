/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/14 16:01:17 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"

int error_response(int code) {
	switch (code) {
		case 400:
			return 400; // Bad Request
		case 411:
			return 411; // Length Required
		case 413:
			return 412; // Content Too Large
		case 501:
			return 501; // Not Implemented
		case 505:
			return 505; // HTTP Version Not Supported
		default:
			return 500; // Internal Server Error
	}
}