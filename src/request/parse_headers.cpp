/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_headers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:19:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/24 14:19:26 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "request_handler.hpp"
#include "support_file.hpp"
#include "Config.hpp"

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
		connection.request.setHost(host);
		connection.request.setPort(80);
		return;
	}
	host_name = host.substr(0, colon_pos);
	connection.request.setHost(host_name);
	port_number = host.substr(colon_pos + 1);
	connection.request.setPort(atoi(port_number.c_str()));
}

int method_check(Connection& connection) {
	std::string	method;

	method = connection.request.getMethod();
	for (std::vector<std::string>::iterator it = connection.server.methods.begin(); it != connection.server.methods.end(); it++) {
		std::cout << *it << std::endl;
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
	header = connection.request.getHeader("keep-alive");
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
			connection.request.setKeepAliveTimeout(atoi(value.c_str()));
		else if (key == "max")
			connection.request.setKeepAliveMax(atoi(value.c_str()));
	}
	return CONTINUE_READ;
}
	

int headers_content_check(Connection& connection, Config& config) {
	std::string	host;
	std::string	contentLength;
	std::string keepAlive;

	host = connection.request.getHeader("host");
	if (host.empty())
		return BAD_REQUEST;
	parse_host(connection, host);
	for (int i = 0; i < config.getNbServers(); i++) {
		t_ServerData server = config.getServerData(i);
		if (server.server_name == connection.request.getHost()
			&& atoi(server.port.c_str()) == connection.request.getPort()) {
			connection.server = server;
			break;
		}
	}
	
	// // Allowed methods untested, to test later
	// if (method_check(connection) == METHOD_NOT_ALLOWED)
	// 	return METHOD_NOT_ALLOWED;

	keepAlive = connection.request.getHeader("connection");
	std::string::size_type comma;
	comma = keepAlive.find(",");
	if (comma != std::string::npos)
		keepAlive = keepAlive.substr(0, comma);
	keepAlive = toLower(keepAlive);
	if (keepAlive == "keep-alive") {
		connection.request.setKeepAlive(keepAlive);
		if (!connection.request.getHeader("keep-alive").empty())
			if (parse_keepAlive(connection) == BAD_REQUEST)
				return BAD_REQUEST;
	} else if (keepAlive == "close")
		connection.request.setKeepAlive(keepAlive);

	contentLength = connection.request.getHeader("content-length");
	if (connection.request.getMethod() == "POST" && contentLength.empty())
		return LENGTH_REQUIRED;
	// Max body size untested, to test later
	if (!contentLength.empty() && connection.request.getMethod() == "POST") {
		connection.request.setContentLength(strtol(contentLength.c_str(), NULL, 10));

		// if (connection.request.getContentLength() > connection.server.client_max_body_size)
		// 	return CONTENT_TOO_LARGE;
		
		connection.state = READING_BODY;
		return READING_BODY;
	}
	return READING_COMPLETE;
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
		if (key == "host" && !connection.request.getHeader("host").empty())
			return BAD_REQUEST;
		size_t start = colon_pos + 1;
		while (start < header.size() && (header[start] == ' ' || header[start] == '\t'))
			start++;
		value = header.substr(start);
		connection.request.setHeader(key, value);
		connection.buffer.erase(0, end_pos + 2);
	}
	return headers_content_check(connection, config);
}
