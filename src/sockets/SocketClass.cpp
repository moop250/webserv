#include "../../headers/StdLibs.hpp"
#include "../../headers/Config.hpp"
#include "../../headers/SocketClass.hpp"
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/socket.h>

ServerSocket::ServerSocket() {}

ServerSocket::~ServerSocket() {}

void ServerSocket::initializeNewSocket_(std::string combo) {
	int status, socketfd;
	struct addrinfo	info, *res;
	int del = combo.find("|");
	std::string		host = combo.substr(0, del),
					port = combo.substr(del + 1, combo.length());


	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(host.empty() ? NULL : host.c_str(),
							port.c_str(),
							&info, &res);

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd < 0) {throw std::runtime_error("server_fd_ socket error:" + std::string(strerror(errno)));}
	
	// Bind the combo to a socket

	status = bind(socketfd, res->ai_addr, res->ai_addrlen);

	freeaddrinfo(res);

	// Start listenting to the socket
	
	// save file descriptor to a class or something to be returned

	this->socketFd_.push_back(socketfd);
};

int ServerSocket::getSocketFd(int pos) {
	return this->socketFd_.at(pos);
}