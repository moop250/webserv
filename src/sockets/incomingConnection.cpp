#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/Connection.hpp"
#include <cerrno>
#include <climits>
#include <poll.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <limits>
#include <unistd.h>

static void addToPollfd(std::vector<pollfd> *fds, int newFD, ServerSocket *sockets, std::map<int, Connection> *connectMap) {
	pollfd newPollFD;
	
	newPollFD.fd = newFD;
	newPollFD.events = POLLIN;
	newPollFD.revents = 0;
	fds->push_back(newPollFD);
	
	Connection	newConnection;
	connectMap->insert({newFD, newConnection});

	sockets->incrementClientCount();
}

static void removeFromPollfd(std::vector<pollfd> *fds, int fd, ServerSocket *sockets, std::map<int, Connection> *connectMap) {
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			fds->erase(it);
			break;
		}
	}
	connectMap->erase(fd);

	sockets->decrementClientCount();
}

static int handleConnection(ServerSocket *sockets, std::vector<pollfd> *fds, int fd, std::map<int, Connection> *connectMap) {
	struct sockaddr_storage newRemote;
	socklen_t               addrLen;
	int remoteFD;
	std::string remoteIP;

	addrLen = sizeof newRemote;
	remoteFD = accept(fd, (struct sockaddr *)&newRemote,&addrLen);
	if (remoteFD == -1) {
		return ACCEPTERROR;
	} else {
		addToPollfd(fds, remoteFD, sockets, connectMap);
		return remoteFD;
	}
};

static int handleClientData(int fd, std::map<int, Connection> *connectMap, Config *conf, char **env) {
	int	code = CONTINUE_READ;
	Connection *connect = &connectMap->at(fd);
	connect->setState(READING_METHOD);
	while (code == CONTINUE_READ) {
		std::string buf(8192, '\0');
	
		int nbytes = recv(fd, &buf[0], buf.size(), 0);

		if (nbytes <= 0) {
			if (nbytes == 0) {
				return HUNGUP;
			} else {
				return RECVERROR;
			}
		}
		buf.resize(nbytes);

		// parsing function here:
		std::cout << buf << std::endl;
		connect->buffer.append(buf);
		code = parse_request(connect, conf, env);
	}
	switch (code) {
		case -1:
			return CLOSEFD;
		case MAKING_RESPONSE:
			// change pollin to pollout
			return CLIENTDATASUCCESS;
	}
};

static bool checkServ(ServerSocket *sockets, int fd) {
	for (int j = 0; j < sockets->getSocketCount(); ++j) {
			if (fd == sockets->getSocketFd(j)) {
				return true;
			}
		}
	return false;
}

static int handlePOLLIN(int fd, ServerSocket *sockets, std::vector<pollfd> *fds, std::map<int, Connection> *connectMap, Config *conf, char **env) {
	if (checkServ(sockets, fd)) {
		int tmp = handleConnection(sockets, fds, fd, connectMap);
		switch (tmp)
		{
			case ACCEPTERROR:
				break ;
			default:
				std::cout << YELLOW << "Accept: New connection on socket: " << tmp << RESET << std::endl;
		}
	} else {
		switch(handleClientData(fd, connectMap, conf, env))
		{
			case CLOSEFD:
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				break;
			case CLIENTDATASUCCESS:
				break;
			case HUNGUP:
				std::cout << YELLOW << "Recv: socket " << fd << " hung up" << RESET << std::endl;
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				return -1;
			case RECVERROR:
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				return -1;
			default:
				std::cout << "handleClientData: default: how did you get here?" << std::endl;
		}
	}
	return 0;
}

static void handlePOLLOUT() {

}

int incomingConnection(ServerSocket *sockets, std::vector<pollfd> *fds, Config *config, char **env, std::map<int, Connection> *connectMap) {
	// testing
	(void)config;
	(void)env;

	for (int i = 0; i < sockets->getTotalSocketCount(); ++i) {
		if ((*fds)[i].revents & (POLLIN | POLLHUP)) {
			if (handlePOLLIN((*fds)[i].fd, sockets, fds, connectMap, config, env) < 0) {
				continue;
			}
		}
		if ((*fds)[i].revents & POLLOUT) {
			handlePOLLOUT();
			connectMap->at((*fds)[i].fd).setState(WAITING_REQUEST);
		}
	}

	return 0;
};