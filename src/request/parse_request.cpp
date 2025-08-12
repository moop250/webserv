/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:05:10 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/12 23:43:11 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"

int parse_request_line(Request request, std::string buffer) {
	if (buffer.empty())
		return 400;
	return 0;
}

int parse_request(std::string buffer) {
	Request	request;
	int		code = 0;
	
	code = parse_request_line(request, buffer);
	if (code != 0)
		return (-1); // construct error response here
	
	return 0;
}