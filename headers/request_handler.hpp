/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/25 14:00:27 by hoannguy         ###   ########.fr       */
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
	READING_CHUNKED = 6,
	CONTINUE_READ = 7,
	READING_COMPLETE = 8
};

struct Connection {
	std::string		buffer;
	ParseState		state;
	Request			request;
	t_ServerData	server;
	long			chunked_size;
};

int		error_response(int code, int fd_client);
int		parse_method(Connection& connection);
int		parse_URL(Connection& connection, Config& config);
int		parse_http_ver(Connection& connection);
int		parse_headers(Connection& connection, Config& config);
int		parse_body_chunked(Connection& connection);
int		parse_body(Connection& connection);
int		parse_request_type(Connection& connection);
int		parse_request(Connection& connection, Config& config, int fd_client, char **env);

void	handle_request(Connection& connection, Config& config, int fd_client, char **env);

#endif