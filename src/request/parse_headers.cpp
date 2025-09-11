/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_headers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:19:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/10 15:51:35 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"
#include "Config.hpp"
#include "RequestServer.hpp"
#include "Connection.hpp"

std::string trim(const std::string &str) {
	std::string::size_type	first;
	std::string::size_type	last;

	first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == std::string::npos)
		return "";
	last = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(first, last - first + 1);
}
std::string toLower(const std::string& str) {
	std::string	lower;

	lower = str;
	for (std::string::size_type i = 0; i < lower.size(); ++i) {
		lower[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower[i])));
	}
	return lower;
}

void parse_host(Connection& connection, std::string& host) {
	std::string::size_type	colon_pos;
	std::string				host_name;
	std::string				port_number;

	colon_pos = host.find(":");
	if (colon_pos == std::string::npos) {
		connection.getRequest().setHost(host);
		connection.getRequest().setPort("80");
		return;
	}
	host_name = host.substr(0, colon_pos);
	connection.getRequest().setHost(host_name);
	port_number = host.substr(colon_pos + 1);
	connection.getRequest().setPort(port_number);
}

int method_check(Connection& connection) {
	std::string					method;
	std::vector<std::string>	allowed;

	method = connection.getRequest().getMethod();
	allowed = connection.getServer().methods();
	std::cout << "method allowed : " << allowed.at(0) << std::endl;
	for (std::vector<std::string>::iterator it = allowed.begin(); it != allowed.end(); it++) {
		if (*it == method)
			return CONTINUE_READ;
	}
	return METHOD_NOT_ALLOWED;
}

int parse_keepAlive(Connection& connection) {
	std::string				header;
	std::string				subHeader;
	std::string				key;
	std::string				value;
	std::string::size_type	comma_pos;
	std::string::size_type	equal_pos;
	std::string::size_type	start_pos;

	start_pos = 0;
	header = connection.getRequest().getHeader("keep-alive");
	while (start_pos < header.size()) {
		comma_pos = header.find(",", start_pos);
		if (comma_pos == std::string::npos) {
			subHeader = header.substr(start_pos);
			start_pos = header.size();
		} else {
			subHeader = header.substr(start_pos, comma_pos - start_pos);
			start_pos = comma_pos + 1;
		}
		while (!subHeader.empty() && (subHeader[0] == ' ' || subHeader[0] == '\t'))
			subHeader.erase(0, 1);
		while (!subHeader.empty() && (subHeader[subHeader.size() - 1] == ' ' || subHeader[subHeader.size() - 1] == '\t'))
			subHeader.erase(subHeader.size() - 1, 1);
		if (subHeader.empty())
			continue;
		equal_pos = subHeader.find("=");
		if (equal_pos == std::string::npos)
			return BAD_REQUEST;
		key = subHeader.substr(0, equal_pos);
		value = subHeader.substr(equal_pos + 1);
		if (key == "timeout")
			connection.getRequest().setKeepAliveTimeout(atoi(value.c_str()));
		else if (key == "max")
			connection.getRequest().setKeepAliveMax(atoi(value.c_str()));
	}
	return CONTINUE_READ;
}

// stuffs to do
void matching_server(Connection& connection, Config& config) {
	std::string	name;
	std::string	path;
	std::string	port;

	name = connection.getRequest().getHost();
	path = connection.getRequest().getPath();
	port = connection.getRequest().getPort();
	RequestServer server(config, name, port, path);
	if (server.isValid() == true) {
		//  std::cout << "\nA SERVER IS MATCHED\n" << std::endl;
		connection.setServer(server);
		// std::cout << connection.getServer() << std::endl;
		// std::cout << "body: " << connection.getServer().errorPages().content(404) << std::endl;
	} else {
		std::cout << "NO SERVER MATCHED" << std::endl;
		// add fall back server
	}
}

int parse_body_chunked(Connection& connection) {
	std::string::size_type	end_pos;
	std::string				line;

	// if (connection.buffer.size() > connection.getServer().clientSize())
	// 		return CONTENT_TOO_LARGE;
	while(true) {
		if (connection.getChunkedSize() == -1) {
			end_pos = connection.buffer.find("\r\n");
			if (end_pos == std::string::npos)
				return CONTINUE_READ;
			line = connection.buffer.substr(0, end_pos);
			connection.buffer.erase(0, end_pos + 2);
			connection.setChunkedSize(strtol(line.c_str(), NULL, 16));
			if (connection.getChunkedSize() < 0) {
				return BAD_REQUEST;
			}
			// if (connection.getChunkedSize() > connection.getServer().client_max_body_size)
			// 	return CONTENT_TOO_LARGE;
			if (connection.getChunkedSize() == 0) {
				connection.buffer.erase(0, end_pos + 4);
				connection.setState(MAKING_RESPONSE);
				return MAKING_RESPONSE;
			}
		}
		if (static_cast<long>(connection.buffer.size()) < connection.getChunkedSize() + 2)
			return CONTINUE_READ;
		connection.getRequest().appendBody(connection.buffer.substr(0, connection.getChunkedSize()));
		connection.buffer.erase(0, connection.getChunkedSize());
		if (connection.buffer.substr(0, 2) != "\r\n")
			return BAD_REQUEST;
		connection.buffer.erase(0, 2);
		connection.setChunkedSize(-1);
	}
}

int content_length_check(Connection& connection) {
	std::string	contentLength;
	std::string	transferEncoding;
	std::string	method;

	if (connection.buffer.size() > connection.getServer().clientSize())
			return CONTENT_TOO_LARGE;
	contentLength = connection.getRequest().getHeader("content-length");
	transferEncoding = connection.getRequest().getHeader("transfer-encoding");
	method = connection.getRequest().getMethod();
	if (contentLength.empty() && transferEncoding.empty())
	{
		if (method == "POST")
			return LENGTH_REQUIRED;
		else
			return MAKING_RESPONSE;
	}
	if (!contentLength.empty() && !transferEncoding.empty())
		return BAD_REQUEST;
	if (!contentLength.empty()) {
		connection.getRequest().setContentLength(strtol(contentLength.c_str(), NULL, 10));
		if (connection.getRequest().getContentLength() <= 0)
			return BAD_REQUEST;
		if (connection.getRequest().getContentLength() > connection.getServer().clientSize())
			return CONTENT_TOO_LARGE;
		connection.setState(READING_BODY);
		return READING_BODY;
	}
	if (!transferEncoding.empty() && transferEncoding == "chunked") {
		connection.setState(READING_CHUNKED);
		return READING_CHUNKED;
	}
	else
		return NOT_IMPLEMENTED;
}

int parse_redirect(Connection& connection, std::string& redirect) {
	std::string				code;
	std::string				redirect_path;
	std::string::size_type	space_pos;

	space_pos = redirect.find(" ");
	if (space_pos == std::string::npos)
		code = redirect;
	else
		code = redirect.substr(0, space_pos);
	if (code == "403") {
		if (space_pos != std::string::npos)
			return INTERNAL_ERROR;
		return FORBIDDEN;
	}
	if (code == "301" || code == "302") {
		if (space_pos == std::string::npos)
			return INTERNAL_ERROR;
		redirect_path = trim(redirect.substr(space_pos + 1));
		if (redirect_path.empty())
			return INTERNAL_ERROR;
		connection.getRequest().setRedirect(redirect_path);
		if (code == "301")
			return MOVED_PERMANENTLY;
		if (code == "302")
			return FOUND;
	}
	return INTERNAL_ERROR;
}

int headers_content_check(Connection& connection, Config& config) {
	std::string	host;
	std::string keepAlive;
	std::string	contentType;
	std::string	redirect;

	host = connection.getRequest().getHeader("host");
	if (host.empty())
		return BAD_REQUEST;
	parse_host(connection, host);
	if (connection.getReconnect() == false)
		matching_server(connection, config);
	redirect = connection.getServer().redirect();
	if (!redirect.empty())
		return parse_redirect(connection, redirect);
	if (method_check(connection) == METHOD_NOT_ALLOWED)
		return METHOD_NOT_ALLOWED;
	keepAlive = connection.getRequest().getHeader("connection");
	std::string::size_type comma;
	comma = keepAlive.find(",");
	if (comma != std::string::npos)
		keepAlive = keepAlive.substr(0, comma);
	keepAlive = toLower(keepAlive);
	if (keepAlive == "keep-alive") {
		connection.getRequest().setKeepAlive(keepAlive);
		if (!connection.getRequest().getHeader("keep-alive").empty())
			if (parse_keepAlive(connection) == BAD_REQUEST)
				return BAD_REQUEST;
	} else if (keepAlive == "close") {
		connection.setClose(true);
		connection.getRequest().setKeepAlive(keepAlive);
	}
	contentType = connection.getRequest().getHeader("content-type");
	if (contentType == "") {
		contentType = "application/octet-stream";
	}
	connection.getRequest().setContentType(contentType);
	return content_length_check(connection);
}

int parse_headers(Connection& connection, Config& config) {
	std::string::size_type	end_pos;
	std::string::size_type	colon_pos;
	std::string				header;
	std::string				key;
	std::string				value;
	
	while (true) {
		end_pos = connection.buffer.find("\r\n");
		if (end_pos == std::string::npos)
			return CONTINUE_READ;
		if (end_pos == 0) {
			connection.buffer.erase(0, 2);
			break;
		}
		header = connection.buffer.substr(0, end_pos);
		colon_pos = header.find(":");
		if (colon_pos == std::string::npos)
			return BAD_REQUEST;
		key = header.substr(0, colon_pos);
		key = toLower(key);
		if (key == "host" && !connection.getRequest().getHeader("host").empty())
			return BAD_REQUEST;
		size_t start = colon_pos + 1;
		while (start < header.size() && (header[start] == ' ' || header[start] == '\t'))
			start++;
		value = header.substr(start);
		connection.getRequest().setHeader(key, value);
		connection.buffer.erase(0, end_pos + 2);
	}
	return headers_content_check(connection, config);
}
