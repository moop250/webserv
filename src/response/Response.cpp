/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 22:31:54 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/19 16:18:41 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Response::Response() {
	this->httpVersion = "HTTP/1.1";
	this->code = 0;
	this->contentLength = 0;
}

Response::~Response() {
	
}



// ----------------- METHODS ----------------------

std::string Response::headersToString() {
	std::string header;
	for (int i = 0; i < this->headers.size(); i++) {
		header += this->headers[i].first;
		header += ": ";
		header += this->headers[i].second;
		header += "\r\n";
	}
	return header;
}
std::string Response::constructResponse() {
	std::string headers = this->headersToString();
	std::string code = std::to_string(this->code);
	return (this->httpVersion + " " + code + " " + this->codeMessage + "\r\n"
			+ headers
			+ "\r\n"
			+ this->body);
}

std::ostream& operator <<(std::ostream& o, Response& response) {
	o << response.constructResponse() << std::endl;
	return o;
}


// ----------------- GETTERS ----------------------

std::string Response::getHttpVersion() const {
	return this->httpVersion;
}
int Response::getCode() const {
	return this->code;
}
std::string Response::getCodeMessage() const {
	return this->codeMessage;
}
std::string Response::getErrorPagePath() const {
	return this->errorPagePath;
}
std::string Response::getHeader(const std::string& key) const {
	for (int i = 0; i < this->headers.size(); i++) {
		if (this->headers[i].first == key)
			return this->headers[i].second;
	}
	return "";
}
std::string Response::getBody() const {
	return this->body;
}
size_t Response::getContentLength() const {
	return this->contentLength;
}
std::string Response::getContentType() const {
	return this->contentType;
}




// ----------------- SETTERS ----------------------

Response& Response::setHttpVersion(const std::string& version) {
	this->httpVersion = version;
	return *this;
}
Response& Response::setCode(const int code) {
	this->code = code;
	return *this;
}
Response& Response::setCodeMessage(const std::string& message) {
	this->codeMessage = message;
	return *this;
}
Response& Response::setErrorPagePath(const std::string& path) {
	this->errorPagePath = path;
	return *this;
}
void Response::setHeader(const std::string& key, const std::string& value) {
	this->headers.push_back(std::make_pair(key, value));
}
Response& Response::setBody(const std::string& body) {
	this->body = body;
	return *this;
}
Response& Response::setContentLength(const size_t len) {
	this->contentLength = len;
	return *this;
}
Response& Response::setContentType(const std::string& type) {
	this->contentType = type;
	return *this;
}

