#include "../../headers/StdLibs.hpp"
#include "../../headers/Config.hpp"
#include "../../headers/SocketClass.hpp"
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/socket.h>

ServerSocket::ServerSocket(std::string combo) : server_fd_(-1) {
	initializeSocket_(combo);
}

ServerSocket::~ServerSocket() {}

void ServerSocket::initializeSocket_(std::string combo) {

	this->server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

	if (this->server_fd_ < 0) {throw std::runtime_error("server_fd_ socket error:" + std::string(strerror(errno)));}
	
	// Bind the combo to a socket

	
	// Start listenting to the socket
	
	// save file descriptor to a class or something to be returned

	// return status code
};

int ServerSocket::getFd() {
	return server_fd_;
}