/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:45 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/12 17:34:32 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <map>
// #include <exception>

enum requestType {
	File,
	CGI,
	Directory,
	None
};

class Request {
	private:
		std::string							method;
		std::string							url;
		std::string							query;
		std::string							httpVersion;
		std::map<std::string, std::string>	headers;
		std::map<std::string, std::string>	cookies;
		std::string							fileType;
		int									requestType;
		std::string							body;
		unsigned long long					contentLength;

	public:
		Request();
		~Request();
		
		std::string			getMethod() const;
		std::string			getURL() const;
		std::string			getQuery() const;
		std::string			getHttpVersion() const;
		std::string			getHeader(const std::string& key) const;
		std::string			getCookie(const std::string& key) const;
		std::string			getFileType() const;
		int					getRequestType() const;
		std::string			getBody() const;
		unsigned long long	getContentLength() const;

		Request&	setMethod(const std::string& method);
		Request&	setURL(const std::string& url);
		Request&	setQuery(const std::string& query);
		Request&	setHttpVersion(const std::string& version);
		void		setHeader(const std::string& key, const std::string& value);
		void		setCookie(const std::string& key, const std::string& value);
		Request&	setFileType(const std::string& fileType);
		Request&	setRequestType(const int type);
		Request&	setBody(const std::string& body);
		Request&	setContentLength(const unsigned long long len);

		// void	setEnv();
};

#endif