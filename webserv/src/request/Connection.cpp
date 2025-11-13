/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:59:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/11/12 18:07:42 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Connection::Connection() {
	this->state = READING_METHOD;
	this->chunked_size = -1;
	this->offset = 0;
	this->reconnect = false;
	this->close = false;
	this->server = getDefaultServer();
	this->fdin = -1;
	this->fdout = -1;
	this->first = 0;
	this->requestType = None;
	this->operation = No;
	this->pid = -1;
}
Connection::Connection(const Connection& copy) {
	*this = copy;
}

Connection& Connection::operator =(const Connection& assign) {
	if (this != &assign) {
		this->state = assign.state;
		this->request = assign.request;
		this->response = assign.response;
		this->server = assign.server;
		this->chunked_size = assign.chunked_size;
		this->offset = assign.offset;
		this->buffer = assign.buffer;
		this->iobuffer = assign.iobuffer;
		this->reconnect = assign.reconnect;
		this->close = assign.close;
		this->port = assign.port;
		this->ip = assign.ip;
		this->fdin = assign.fdin;
		this->fdout = assign.fdout;
		this->first = assign.first;
		this->requestType = assign.requestType;
		this->operation = assign.operation;
		this->pid = assign.pid;
		this->cgi_output = assign.cgi_output;
		this->cgi_time = assign.cgi_time;
	}
	return *this;
}

Connection::~Connection() {
	
}




// ----------------- METHODS ----------------------
Connection& Connection::minusOffset(long size) {
	this->offset -= size;
	return *this;
}

Connection& Connection::plusOffset(long size) {
	this->offset += size;
	return *this;
}

Connection& Connection::clear() {
	this->state = WAITING_REQUEST;
	this->request.clear();
	this->response.clear();
	this->chunked_size = -1;
	this->offset = 0;
	this->buffer.clear();
	this->iobuffer.clear();
	this->reconnect = true;
	this->close = false;
	// Server not reset!
	// Port and IP not reset!
	this->fdin = -1;
	this->fdout = -1;
	this->first = 0;
	this->requestType = None;
	this->operation = No;
	this->pid = -1;
	this->cgi_output.clear();
	this->cgi_time = 0;
	return *this;
}



// ----------------- GETTERS ----------------------

State Connection::getState() const {
	return this->state;
}
Request& Connection::getRequest() {
	return this->request;
}
Response& Connection::getResponse() {
	return this->response;
}
RequestServer Connection::getServer() const {
	return this->server;
}
long Connection::getChunkedSize() const {
	return this->chunked_size;
}
long Connection::getOffset() const {
	return this->offset;
}
bool Connection::getReconnect() const {
	return this->reconnect;
}
bool Connection::getClose() const {
	return this->close;
}
std::string Connection::getPort() const {
	return this->port;
}
std::string Connection::getIP() const {
	return this->ip;
}
int Connection::getFDIN() const {
	return this->fdin;
}
int Connection::getFDOUT() const {
	return this->fdout;
}
int Connection::getFirst() const {
	return this->first;
}
int Connection::getRequestType() const {
	return this->requestType;
}
Operation Connection::getOperation() const {
	return this->operation;
}
pid_t Connection::getPid() const {
	return this->pid;
}
std::string Connection::get_CgiOutput() const {
	return this->cgi_output;
}
std::time_t Connection::get_CgiTime() const {
	return this->cgi_time;
}

#include <dirent.h>
#include <stdio.h>

RequestServer	Connection::getDefaultServer() const {
	std::cout << "Giving default configuration\n";
	return RequestServer(1);
}

// ----------------- SETTERS ----------------------

Connection& Connection::setState(const State state) {
	this->state = state;
	return *this;
}
Connection& Connection::setServer(const RequestServer& server) {
	this->server = server;
	return *this;
}
Connection& Connection::setChunkedSize(const long size) {
	this->chunked_size = size;
	return *this;
}
Connection& Connection::setOffset(const long size) {
	this->offset = size;
	return *this;
}
Connection& Connection::setReconnect(const bool reconnect) {
	this->reconnect = reconnect;
	return *this;
}
Connection& Connection::setClose(const bool close) {
	this->close = close;
	return *this;
}
Connection& Connection::setPort(const std::string port) {
	this->port = port;
	return *this;
}
Connection& Connection::setIP(const std::string ip) {
	this->ip = ip;
	return *this;
}
Connection& Connection::setFDIN(const int fdin) {
	this->fdin = fdin;
	return *this;
}
Connection& Connection::setFDOUT(const int fdout) {
	this->fdout = fdout;
	return *this;
}
Connection& Connection::setFirst(const int first) {
	this->first = first;
	return *this;
}
Connection& Connection::setRequestType(const int requestType) {
	this->requestType = requestType;
	return *this;
}
Connection& Connection::setOperation(const Operation operation) {
	this->operation = operation;
	return *this;
}
Connection& Connection::setPid(const pid_t pid) {
	this->pid = pid;
	return *this;
}
Connection& Connection::appendCGIoutput(const char *buffer, long size) {
	this->cgi_output.append(buffer, size);
	return *this;
}
Connection& Connection::setCGIoutput(const std::string cgioutput) {
	this->cgi_output = cgioutput;
	return *this;
}
Connection& Connection::setCGItime(const std::time_t time) {
	this->cgi_time = time;
	return *this;
}