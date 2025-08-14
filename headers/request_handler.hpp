/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/14 17:43:05 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "Request.hpp"

int error_response(int code);
int parse_method(Request& request, std::string& buffer);
int parse_URL(Request& request, std::string& buffer);
int parse_http_ver(Request& request, std::string& buffer);
int parse_headers(Request& request, std::string& buffer);
int parse_body(Request& request, std::string& buffer);
int parse_request_type(Request& request);
int parse_request(std::string& buffer);

int handle_request(Request& request);

#endif