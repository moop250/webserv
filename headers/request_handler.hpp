/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:17 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "Request.hpp"
#include "Config.hpp"

enum ParseState {
	READING_METHOD = 1,
	READING_PATH = 2,
	READING_HTTPVERSION = 3,
	READING_HEADERS = 4,
	READING_BODY = 5,
	REQUEST_COMPLETE = 6
};

struct Connection {
	std::string buffer;
	ParseState state;
	Request request;
	size_t body_bytes_read;
};

int error_response(int code, int fd_client);
int parse_method(Connection& connection);
int parse_URL(Connection& connection, Config& config);
int parse_http_ver(Connection& connection);

int parse_headers(Request& request, std::string& buffer);
int parse_body(Request& request, std::string& buffer);
int parse_request_type(Request& request);

int parse_request(Connection& connection, Config& config, int fd_client, char **env);

int handle_request(Request& request);

#endif