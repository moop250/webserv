/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:59:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/01 17:21:50 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Connection::Connection() {
	this->state = READING_METHOD;
	this->chunked_size = -1;
	this->offset = -2;
}

Connection::~Connection() {
	
}




// ----------------- METHODS ----------------------
Connection&		Connection::minusOffset(long size) {
	this->offset -= size;
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



// ----------------- SETTERS ----------------------

Connection& Connection::setState(State state) {
	this->state = state;
	return *this;
}
Connection& Connection::setServer(RequestServer& server) {
	this->server = server;
	return *this;
}
Connection& Connection::setChunkedSize(long size) {
	this->chunked_size = size;
	return *this;
}
Connection& Connection::setOffset(long size) {
	this->offset = size;
	return *this;
}