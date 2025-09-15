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

ServerSocket::ServerSocket() : clientCount_(0) {}

ServerSocket::~ServerSocket() {
	if (this->getSocketCount() <= 0)
		return;
	for (int i = 0; i < this->getSocketCount(); ++i) {
		close(this->getSocketFd(i));
	}
}

//void	ServerSocket::initializeNewSocket(std::string combo) {
//	int socketfd = -1;
//	struct addrinfo	info, *res;
//	size_t del = combo.find("|");
//	if (del == std::string::npos)
//		{throw std::runtime_error("Socket initialization error: incomplete combo");}
//	std::string		host = combo.substr(0, del),
//					port = combo.substr(del + 1, combo.length());
//
//	memset(&info, 0, sizeof(info));
//
//	info.ai_family = AF_INET;
//	info.ai_socktype = SOCK_STREAM;
//
//	if (getaddrinfo(host.c_str(), port.c_str(), &info, &res) < 0)
//		{throw std::runtime_error("getaddrinfo error: " + std::string(strerror(errno)));}
//	socketfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (socketfd < 0)
//		{throw std::runtime_error("Socket initialization error: " + std::string(strerror(errno)));}
//	if (bind(socketfd, res->ai_addr, res->ai_addrlen) < 0)
//		{throw std::runtime_error("Socket binding error: combo: " + combo + " : " + std::string(strerror(errno)));}
//
//	freeaddrinfo(res);
//	
//	if (listen(socketfd,BACKLOG) < 0)
//		{throw std::runtime_error("Socket listen error: " + std::string(strerror(errno)));}
//
//	this->serverSocketFd_.push_back(socketfd);
//	this->serverPort_.push_back(atoi(port.c_str()));
//};
//
void ServerSocket::initializeNewSocket(std::string combo) {
    int socketfd = -1;
    struct addrinfo info, *res;
    size_t del = combo.find("|");

    if (del == std::string::npos) {
        throw std::runtime_error("Socket initialization error: incomplete combo");
    }

    std::string host = combo.substr(0, del);
    std::string port = combo.substr(del + 1, combo.length());

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &info, &res) < 0) {
        throw std::runtime_error("getaddrinfo error: " + std::string(strerror(errno)));
    }

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Socket initialization error: " + std::string(strerror(errno)));
    }

    // activer SO_REUSEADDR avant bind() (pr relancer ws oklm)
	//	debut modif
    int opt = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(socketfd);
        freeaddrinfo(res);
        throw std::runtime_error("setsockopt(SO_REUSEADDR) failed: " + std::string(strerror(errno)));
    }	// fin de modif

    if (bind(socketfd, res->ai_addr, res->ai_addrlen) < 0) {
        close(socketfd);
        freeaddrinfo(res);
        throw std::runtime_error("Socket binding error: combo: " + combo + " : " + std::string(strerror(errno)));
    }

    freeaddrinfo(res);

    if (listen(socketfd, BACKLOG) < 0) {
        close(socketfd);
        throw std::runtime_error("Socket listen error: " + std::string(strerror(errno)));
    }

    this->serverSocketFd_.push_back(socketfd);
    this->serverPort_.push_back(atoi(port.c_str()));
}

int	ServerSocket::getSocketFd(int pos) {
	return this->serverSocketFd_.at(pos);
}

int	ServerSocket::getSocketCount() {
	return this->serverSocketFd_.size();
}

int	ServerSocket::getTotalSocketCount() {
	return this->serverSocketFd_.size() + this->clientCount_;
}

int	ServerSocket::getSocketPort(int pos) {
	return this->serverPort_.at(pos);
}

void	ServerSocket::incrementClientCount() {
	++this->clientCount_;
}

void	ServerSocket::decrementClientCount() {
	--this->clientCount_;
}