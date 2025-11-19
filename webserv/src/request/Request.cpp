/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:49:35 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/19 09:43:42 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"


// -------- CONSTRUCTORS/DESTRUCTORS --------------

Request::Request() {
	this->requestType = None;
	this->contentLength = 0;
	this->keepAliveTimeout = -1;
	this->keepAliveMax = -1;
}
Request::Request(const Request& copy) {
	*this = copy;
}
Request& Request::operator =(const Request& assign) {
	if (this != &assign) {
		this->method = assign.method;
		this->path = assign.path;
		this->basePath = assign.basePath;
		this->query = assign.query;
		this->httpVersion = assign.httpVersion;
		this->headers = assign.headers;
		this->cookies = assign.cookies;
		this->requestType = assign.requestType;
		this->fileType = assign.fileType;
		this->body = assign.body;
		this->host = assign.host;
		this->port = assign.port;
		this->contentLength = assign.contentLength;
		this->contentType = assign.contentType;
		this->keepAlive = assign.keepAlive;
		this->keepAliveTimeout = assign.keepAliveTimeout;
		this->keepAliveMax = assign.keepAliveMax;
		this->redirect = assign.redirect;
	}
	return *this;
}
Request::~Request() {
	
}

// ----------------- METHODS ----------------------

Request& Request::appendBody(const std::string& line) {
	this->body += line;
	return *this;
}

Request& Request::clear() {
	this->method.clear();
	this->path.clear();
	this->basePath.clear();
	this->query.clear();
	this->httpVersion.clear();
	this->headers.clear();
	this->cookies.clear();
	this->requestType = None;
	this->fileType.clear();
	this->body.clear();
	this->host.clear();
	this->port.clear();
	this->contentLength = 0;
	this->contentType.clear();
	this->keepAlive.clear();
	this->keepAliveTimeout = -1;
	this->keepAliveMax = -1;
	this->redirect.clear();
	return *this;
}

// ----------------- GETTERS ----------------------

std::string Request::getMethod() const {
		return this->method;
}
std::string Request::getPath() const {
		return this->path;
}
std::string Request::getBasePath() const {
		return this->basePath;
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
	return this->body;
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
std::string Request::getPort() const {
	return this->port;
}
std::string Request::getRedirect() const {
	return this->redirect;
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
Request& Request::setBasePath(const std::string& basePath) {
	this->basePath = basePath;
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
void Request::removeBody(const int pos1, const int pos2) {
	this->body.erase(pos1, pos2);
}
Request& Request::setContentLength(const size_t len) {
	this->contentLength = len;
	return *this;
}
Request& Request::setContentType(const std::string& type) {
	this->contentType = type;
	return *this;
}
Request& Request::setKeepAlive(const std::string& config) {
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
Request& Request::setHost(const std::string& host) {
	this->host= host;
	return *this;
}
Request& Request::setPort(const std::string& port) {
	this->port = port;
	return *this;
}
Request& Request::setRedirect(const std::string& redirect) {
	this->redirect = redirect;
	return *this;
}