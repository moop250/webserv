/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 23:35:36 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/24 10:49:38 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP
#include "Request.hpp"
#include "Config.hpp"
#include "RequestServer.hpp"
#include "Connection.hpp"
#include "Sockets.hpp"

void		error_response(Connection& connection, int code);

int			parse_method(Connection& connection);
int			parse_URL(Connection& connection);
int			parse_http_ver(Connection& connection);
int			parse_headers(Connection& connection, Config& config);
int			parse_body_chunked(Connection& connection);
int			parse_body(Connection& connection);
int			parse_request(Connection& connection, Config& config);

int			handle_request(int fd, t_fdInfo *fdInfo, Connection& connection);
int			file_handler(int fd, t_fdInfo *fdInfo, Connection& connection);
int			get_file(Connection& connection);
int			post_file(Connection& connection);
int			delete_file(Connection& connection);
int			directory_handler(int originFD, t_fdInfo *fdInfo, Connection& connection);
int			get_directory(Connection& connection);
int			post_directory(Connection& connection);
int			delete_directory(Connection& connection);
int			CGI_handler(int fd, t_fdInfo *fdInfo, Connection& connection);
int         parse_cgi_output(Connection& connection, std::string& output);

std::string	size_to_string(size_t size);
std::string	trim(const std::string &str);
std::string	toLower(const std::string& str);
std::string	generate_name(const std::string& extension);
std::string error_message(int code);
std::string error_message(std::string& code);

#endif