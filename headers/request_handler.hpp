/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/07 12:30:49 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "Request.hpp"
#include "Config.hpp"
#include "RequestServer.hpp"
#include "Connection.hpp"

void		error_response(Connection& connection, int code);

int			parse_method(Connection& connection);
int			parse_URL(Connection& connection);
int			parse_http_ver(Connection& connection);
int			parse_headers(Connection& connection, Config& config);
int			parse_body_chunked(Connection& connection);
int			parse_body(Connection& connection);
int			parse_request(Connection& connection, Config& config, char **env);

int			handle_request(Connection& connection, char **env);
int			file_handler(Connection& connection);
int			get_file(Connection& connection);
int			post_file(Connection& connection);
int			delete_file(Connection& connection);
int			directory_handler(Connection& connection);
int			get_directory(Connection& connection);
int			post_directory(Connection& connection);
int			delete_directory(Connection& connection);
int			CGI_handler(Connection& connection, char** env);

std::string	size_to_string(size_t size);
std::string	trim(const std::string &str);
std::string	toLower(const std::string& str);
std::string	generate_name(const std::string& extension);

#endif