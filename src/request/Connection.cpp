/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:59:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/04 16:42:32 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Connection::Connection() {
	this->state = READING_METHOD;
	this->chunked_size = -1;
	this->offset = -2;
	this->reconnect = false;
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
	// Server not reset!
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