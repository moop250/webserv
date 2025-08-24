/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/24 14:09:31 by hoannguy         ###   ########.fr       */
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
	CONTINUE_READ = 6,
	READING_COMPLETE = 7,
};

struct Connection {
	std::string		buffer;
	ParseState		state;
	Request			request;
	size_t			body_bytes_read;
	t_ServerData	server;
};

int		error_response(int code, int fd_client);
int		parse_method(Connection& connection);
int		parse_URL(Connection& connection, Config& config);
int		parse_http_ver(Connection& connection);
int		parse_headers(Connection& connection, Config& config);
int		parse_body(Connection& connection);
int		parse_request_type(Connection& connection);
int		parse_request(Connection& connection, Config& config, int fd_client, char **env);

void	handle_request(Connection& connection, Config& config, int fd_client, char **env);

#endif