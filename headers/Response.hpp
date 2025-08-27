/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 22:18:51 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/27 04:56:43 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <ctime>
#include <sys/socket.h>
#include "Request.hpp"
#include "support_file.hpp"

class Response {
	private:
		std::string 										httpVersion;
		int													code;
		std::string											codeMessage;
		std::vector<std::pair<std::string, std::string> >	headers;
		std::string											body;
		size_t												contentLength;
		std::string											contentType;
	
	public:
		Response();
		~Response();

		std::string	getHttpVersion() const;
		int			getCode() const;
		std::string	getCodeMessage() const;
		std::string	getHeader(const std::string& key) const;
		std::string	getBody() const;
		size_t		getContentLength() const;
		std::string	getContentType() const;

		Response&	setHttpVersion(const std::string& version);
		Response&	setCode(const int code);
		Response&	setCodeMessage(const std::string& message);
		void		setHeader(const std::string& key, const std::string& value);
		Response&	setBody(const std::string& body);
		Response&	setContentLength(const size_t len);
		Response&	setContentType(const std::string& type);

		// to do: get error path 
		std::string	headersToString();
		std::string	constructResponse();
		int			sendResponse(int fd_client);
};

std::ostream& operator <<(std::ostream& o, Response& response);
std::string getCurrentHttpTime();

#endif