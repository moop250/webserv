/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/28 12:55:27 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "Request.hpp"
#include "Config.hpp"
#include "Connection.hpp"

void	error_response(Connection& connection, int code);
int		parse_method(Connection& connection);
int		parse_URL(Connection& connection, Config& config);
int		parse_http_ver(Connection& connection);
int		parse_headers(Connection& connection, Config& config);
int		parse_body_chunked(Connection& connection);
int		parse_body(Connection& connection);
int		parse_request_type(Connection& connection);
int		parse_request(Connection& connection, Config& config, char **env);

int		handle_request(Connection& connection, Config& config, char **env);
int		file_handler(Connection& connection);
int		get_file(Connection& connection);
int		post_file(Connection& connection);
int		delete_file(Connection& connection);


#endif