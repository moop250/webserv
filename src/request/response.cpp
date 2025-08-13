/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:22:50 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 16:58:17 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"

int response(int code) {
	switch (code) {
		case 400:
			return 400; // Bad Request
		case 501:
			return 501; // Not Implemented
		default:
			return 500; // Internal Server Error
	}
}