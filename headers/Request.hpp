/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:45 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/18 22:53:51 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <map>
#include <stdexcept> // std::out_of_range
#include <stdlib.h> // strtol()
#include <limits.h> // LONG_MAX
#include <sys/stat.h>

enum requestType {
	File,
	CGI,
	Directory,
	None
};

class Request {
	private:
		std::string							method;
		std::string							path;
		std::string							query;
		std::string							httpVersion;
		std::map<std::string, std::string>	headers;
		std::map<std::string, std::string>	cookies;
		std::string							cgiType;
		int									requestType;
		std::string							body;
		size_t								contentLength;
		std::string							contentType;

	public:
		Request();
		~Request();
		
		std::string	getMethod() const;
		std::string	getPath() const;
		std::string	getQuery() const;
		std::string	getHttpVersion() const;
		std::string	getHeader(const std::string& key) const;
		std::string	getCookie(const std::string& key) const;
		std::string	getCgiType() const;
		int			getRequestType() const;
		std::string	getBody() const;
		size_t		getContentLength() const;
		std::string	getContentType() const;

		Request&	setMethod(const std::string& method);
		Request&	setPath(const std::string& path);
		Request&	setQuery(const std::string& query);
		Request&	setHttpVersion(const std::string& version);
		void		setHeader(const std::string& key, const std::string& value);
		void		setCookie(const std::string& key, const std::string& value);
		Request&	setCgiType(const std::string& cgiType);
		Request&	setRequestType(const int type);
		Request&	setBody(const std::string& body);
		Request&	setContentLength(const size_t len);
		Request&	setContentType(const std::string& type);

		// void	setEnv();
};

#endif