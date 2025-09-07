/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:45 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/07 11:12:32 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <map>
#include <stdexcept>	// std::out_of_range
#include <stdlib.h>		// strtol()
#include <limits.h>		// LONG_MAX
#include <cctype>
#include <algorithm>
#include <fstream>
#include <unistd.h>		// access(), open()
#include <sys/stat.h>	// stat()
#include <errno.h>
#include <dirent.h>		// opendir(), readdir(), closedir()
#include <sys/types.h>	// complement to dirent.h

#define MOVED_PERMANENTLY 301
#define FOUND 302
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define LENGTH_REQUIRED 411
#define CONTENT_TOO_LARGE 413
#define UNSUPPORTED_MEDIA_TYPE 415
#define INTERNAL_ERROR 500
#define NOT_IMPLEMENTED 501
#define HTTP_VERSION_MISMATCH 505

#define NO_CONTENT 204

enum RequestError
{
	REQUEST_ERROR_BAD_REQUEST,
	REQUEST_ERROR_NOT_FOUND,
	REQUEST_ERROR_NOT_ALLOWED,
	REQUEST_ERROR_LENGTH,
	REQUEST_ERROR_TOO_LARGE,
	REQUEST_ERROR_NOT_IMPLEMENTED,
	REQUEST_ERROR_HTTP_MISSMATCH,
	REQUEST_ERROR_INTERNAL_ERROR,
	OTHER
};

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
		int									requestType;
		std::string							fileType;
		std::string							body;
		std::string							host;
		std::string							port;
		size_t								contentLength;
		std::string							contentType;
		std::string							keepAlive;
		int									keepAliveTimeout;
		int									keepAliveMax;
		std::string							redirect;

	public:
		Request();
		Request(const Request& copy);
		Request& operator =(const Request& assign);
		~Request();
		
		std::string	getMethod() const;
		std::string	getPath() const;
		std::string	getQuery() const;
		std::string	getHttpVersion() const;
		std::string	getHeader(const std::string& key) const;
		std::string	getCookie(const std::string& key) const;
		std::string	getFileType() const;
		int			getRequestType() const;
		std::string	getBody() const;
		size_t		getContentLength() const;
		std::string	getContentType() const;
		std::string	getKeepAlive() const;
		int			getKeepAliveTimeout() const;
		int			getKeepAliveMax() const;
		std::string	getHost() const;
		std::string	getPort() const;
		std::string getRedirect() const;

		Request&	setMethod(const std::string& method);
		Request&	setPath(const std::string& path);
		Request&	setQuery(const std::string& query);
		Request&	setHttpVersion(const std::string& version);
		void		setHeader(const std::string& key, const std::string& value);
		void		setCookie(const std::string& key, const std::string& value);
		Request&	setFileType(const std::string& fileType);
		Request&	setRequestType(const int type);
		Request&	setBody(const std::string& body);
		Request&	setContentLength(const size_t len);
		Request&	setContentType(const std::string& type);
		Request&	setKeepAlive(const std::string& config);
		Request&	setKeepAliveTimeout(const int time);
		Request&	setKeepAliveMax(const int time);
		Request&	setHost(const std::string& host);
		Request&	setPort(const std::string& port);
		Request&	setRedirect(const std::string& redirect);

		// static void	setEnv(Connection& connection, char **env);
		Request&	appendBody(const std::string& line);
		Request&	clear();
};

#endif