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
#include "unistd.h"

ServerSocket::ServerSocket() {}

ServerSocket::~ServerSocket() {
	if (this->getSocketCount() <= 0)
		return;
	for (int i = this->getSocketCount(); i <= 0; --i) {
		close(this->getSocketFd(i));
	}
}

void ServerSocket::initializeNewSocket_(std::string combo) {
	int socketfd = -1, status = -1;
	struct addrinfo	info, *res;
	int del = combo.find("|");
	std::string		host = combo.substr(0, del),
					port = combo.substr(del + 1, combo.length());

	memset(&info, 0, sizeof(info));

	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(host.c_str(), port.c_str(), &info, &res)) < 0)
		{throw std::runtime_error("getaddrinfo error:" + std::string(strerror(errno)));}
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!socketfd)
		{throw std::runtime_error("Socket initialization error:" + std::string(strerror(errno)));}
	if ((status = bind(socketfd, res->ai_addr, res->ai_addrlen)) < 0)
		{throw std::runtime_error("Socket binding error:" + std::string(strerror(errno)));}

	freeaddrinfo(res);
	
	// Start listenting to the socket
	if (listen(socketfd,BACKLOG) < 0)
		{throw std::runtime_error("Socket listen error:" + std::string(strerror(errno)));}

	this->socketFd_.push_back(socketfd);
};

int ServerSocket::getSocketFd(int pos) {
	return this->socketFd_.at(pos);
}

int ServerSocket::getSocketCount() {
	return this->socketFd_.size();
}