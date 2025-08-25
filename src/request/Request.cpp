/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:35 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/25 13:32:13 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"


// -------- CONSTRUCTORS/DESTRUCTORS --------------

Request::Request() {
	this->requestType = None;
	this->contentLength = 0;
	this->keepAliveTimeout = -1;
	this->keepAliveMax = -1;
	this->port = -1;
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
std::string Request::getCgiType() const {
		return this->cgiType;
}
int Request::getRequestType() const {
	return this->requestType;
}
std::string Request::getBody() const {
	if (!this->body.empty() && this->method == "POST")
		return this->body;
	return "";
}
size_t Request::getContentLength() const {
	return this->contentLength;
}
std::string Request::getContentType() const {
	return this->contentType;
}
std::string Request::getKeepAlive() const {
	return this->keepAlive;
}
int Request::getKeepAliveTimeout() const {
	return this->keepAliveTimeout;
}
int Request::getKeepAliveMax() const {
	return this->keepAliveMax;
}
std::string Request::getHost() const {
	return this->host;
}
int Request::getPort() const {
	return this->port;
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
Request& Request::setCgiType(const std::string& cgiType) {
	this->cgiType = cgiType;
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
Request& Request::appendBody(const std::string& line) {
	this->body += line;
	return *this;
}
Request& Request::setContentLength(const size_t len) {
	this->contentLength = len;
	return *this;
}
Request& Request::setContentType(const std::string& type) {
	this->contentType = type;
	return *this;
}
Request& Request::setKeepAlive(const std::string config) {
	this->keepAlive = config;
	return *this;
}
Request& Request::setKeepAliveTimeout(const int time) {
	this->keepAliveTimeout = time;
	return *this;
}
Request& Request::setKeepAliveMax(const int time) {
	this->keepAliveMax = time;
	return *this;
}
Request& Request::setHost(const std::string host) {
	this->host= host;
	return *this;
}
Request& Request::setPort(const int port) {
	this->port = port;
	return *this;
}