/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 22:31:54 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/28 13:34:56 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Response::Response() {
	this->httpVersion = "HTTP/1.1";
	this->setHeader("Date", getCurrentHttpTime());
	this->setHeader("Server", "42_Webserv/1.0.0");
	this->setHeader("Authors", "hoannguy, hlibine, aseite");
	this->code = -1;
	this->contentLength = 0;
}

Response::~Response() {
	
}



// ----------------- METHODS ----------------------

std::string Response::headersToString() {
	std::string header;
	for (size_t i = 0; i < this->headers.size(); i++) {
		header += this->headers[i].first;
		header += ": ";
		header += this->headers[i].second;
		header += "\r\n";
	}
	return header;
}

// Likely to be remake to support chunked encoding
std::string Response::constructResponse() {
	std::string headers;
	std::string code;
	std::ostringstream temp;
	
	if (this->httpVersion == "" || this->code == 0 || this->codeMessage == "")
		return "";
	headers = this->headersToString();
	temp << this->code;
	code = temp.str();
	this->responseComplete = this->httpVersion + " " + code + " " + this->codeMessage + "\r\n"
							+ headers
							+ "\r\n"
							+ this->body;
	return this->responseComplete;
}

// Untested, likely to be remake to communicate with poll() and support chunked encoding
int Response::sendResponse(int fd_client) {
	const char*	response;
	int			sent_total;
	int			bytes_left;
	int			sent;

	response = this->constructResponse().c_str();
	bytes_left = this->constructResponse().size();
	sent_total = 0;
	while (sent_total < bytes_left) {
		sent = send(fd_client, response, bytes_left, 0);
		if (sent == -1)
			break;
		sent_total += sent;
		bytes_left -= sent;
	}
	return sent;
}

std::ostream& operator <<(std::ostream& o, Response& response) {
	o << response.constructResponse() << std::endl;
	return o;
}

std::string getCurrentHttpTime() {
	std::time_t			t;
	std::tm				*tm;
	char				time[100];
	
	t = std::time(NULL);
	tm = std::gmtime(&t);
    if (std::strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S GMT", tm)) {
        return std::string(time);
    }
	return "";
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
std::string Response::getHeader(const std::string& key) const {
	for (size_t i = 0; i < this->headers.size(); i++) {
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
std::string Response::getResponseComplete() const {
	return this->responseComplete;
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

