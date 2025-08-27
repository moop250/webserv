/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:59:49 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/27 07:37:16 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

// -------- CONSTRUCTORS/DESTRUCTORS --------------

Connection::Connection(int fd_client) : fd_client(fd_client) {
	this->state = READING_METHOD;
	this->chunked_size = -1;
}

Connection::~Connection() {
	
}




// ----------------- METHODS ----------------------


// ----------------- GETTERS ----------------------

int Connection::getFdClient() const {
	return this->fd_client;
}
State Connection::getState() const {
	return this->state;
}
Request& Connection::getRequest() {
	return this->request;
}
Response& Connection::getResponse() {
	return this->response;
}
t_ServerData Connection::getServer() const {
	return this->server;
}
long Connection::getChunkedSize() const {
	return this->chunked_size;
}



// ----------------- SETTERS ----------------------

Connection& Connection::setState(State state) {
	this->state = state;
	return *this;
}
Connection& Connection::setServer(t_ServerData& server) {
	this->server = server;
	return *this;
}
Connection& Connection::setChunkedSize(long size) {
	this->chunked_size = size;
	return *this;
}