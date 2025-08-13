/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:35 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/13 22:45:56 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Request::Request() {
	this->requestType = None;
	this->contentLength = 0;
}
Request::~Request() {
	
}


// ----------------- GETTERS ----------------------

std::string Request::getMethod() const {
		return this->method;
}
std::string Request::getPath() const {
		return this->path;
}
std::string Request::getQuery() const {
		return this->query;
}
std::string Request::getHttpVersion() const {
		return this->httpVersion;
}
std::string Request::getHeader(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = this->headers.find(key);
	if (it != this->headers.end()) 
		return (it -> second);
	return "";
}
std::string Request::getCookie(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = this->cookies.find(key);
	if (it != this->cookies.end()) 
		return (it -> second);
	return "";
}
std::string Request::getFileType() const {
		return this->fileType;
}
int Request::getRequestType() const {
	return this->requestType;
}
std::string Request::getBody() const {
	if (!this->body.empty() && this->method == "POST")
		return this->body;
	return "";
}
unsigned long long Request::getContentLength() const{
	if (this->method == "POST")
		return this->contentLength;
	return 0;
}


// ----------------- SETTERS ----------------------

Request& Request::setMethod(const std::string& method) {
	this->method = method;
	return *this;
}
Request& Request::setPath(const std::string& path) {
	this->path = path;
	return *this;
}
Request& Request::setQuery(const std::string& query) {
	this->query = query;
	return *this;
}
Request& Request::setHttpVersion(const std::string& version) {
	this->httpVersion = version;
	return *this;
}
void Request::setHeader(const std::string& key, const std::string& value) {
	this->headers[key] = value;
}
void Request::setCookie(const std::string& key, const std::string& value) {
	this->cookies[key] = value;
}
Request& Request::setFileType(const std::string& fileType) {
	this->fileType = fileType;
	return *this;
}
Request& Request::setRequestType(const int type) {
	this->requestType = type;
	return *this;
}
Request& Request::setBody(const std::string& body) {
	this->body = body;
	return *this;
}
Request& Request::setContentLength(const unsigned long long len) {
	this->contentLength = len;
	return *this;
}