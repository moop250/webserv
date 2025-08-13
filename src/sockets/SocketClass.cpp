#include "../../headers/StdLibs.hpp"
#include "../../headers/Config.hpp"
#include "../../headers/SocketClass.hpp"
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/socket.h>

ServerSocket::ServerSocket() {}

ServerSocket::~ServerSocket() {}

void ServerSocket::initializeNewSocket_(std::string combo) {
	t_socketData currentSocket;
	std::string host, port;
	int del = combo.find("|");

	host = combo.substr(0, del);
	port = combo.substr(del, combo.length());

	currentSocket.server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (currentSocket.server_fd < 0) {throw std::runtime_error("server_fd_ socket error:" + std::string(strerror(errno)));}
	
	// Bind the combo to a socket

	// Start listenting to the socket
	
	// save file descriptor to a class or something to be returned

	this->socketData_.push_back(currentSocket);
};

t_socketData ServerSocket::getSocketData(int pos) {
	return this->socketData_.at(pos);
}