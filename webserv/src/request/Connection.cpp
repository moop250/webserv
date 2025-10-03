/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:59:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/03 14:03:47 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

// stuffs to do
// set default server
Connection::Connection() {
	this->state = READING_METHOD;
	this->chunked_size = -1;
	this->offset = -2;
	this->reconnect = false;
	this->close = false;
	this->server = getDefaultServer();
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
		this->reconnect = assign.reconnect;
		this->close = assign.close;
		this->port = assign.port;
		this->ip = assign.ip;
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
	this->offset = -2;
	this->buffer.clear();
	this->reconnect = true;
	this->close = false;
	// Server not reset!
	// Port and IP not reset!
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