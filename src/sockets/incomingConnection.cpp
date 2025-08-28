#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include <cerrno>
#include <climits>
#include <poll.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <limits>
#include <unistd.h>

static void addToPollfd(std::vector<pollfd> *fds, int newFD, ServerSocket *sockets) {
	pollfd newPollFD;

	newPollFD.fd = newFD;
	newPollFD.events = POLLIN;
	newPollFD.revents = 0;
	fds->push_back(newPollFD);

	sockets->incrementClientCount();
}

static void removeFromPollfd(std::vector<pollfd> *fds, int fd, ServerSocket *sockets) {
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			fds->erase(it);
			break;
		}
	}

	sockets->decrementClientCount();
}

static int handleConnection(ServerSocket *sockets, std::vector<pollfd> *fds, int fd) {

	// accept new client connction
	struct sockaddr_storage newRemote;
	socklen_t               addrLen;
	int remoteFD;
	std::string remoteIP;

	addrLen = sizeof newRemote;
	remoteFD = accept(fd, (struct sockaddr *)&newRemote,&addrLen);
	if (remoteFD == -1) {
		return ACCEPTERROR;
	} else {
		addToPollfd(fds, remoteFD, sockets);
		return CONNECTIONSUCCESS;
	}
};

static int handleClientData(int fd, Config *config, char **env) {
	// testing
	(void) config;
	(void) env;

	std::string buf(8192, '\0');

	int nbytes = recv(fd, &buf[0], buf.size(), 0);

	if (nbytes <= 0) {
		if (nbytes == 0) {
			std::cout << "Recv: socket " << fd << " hung up" << std::endl;
			return HUNGUP;
		} else {
			return RECVERROR;
		}
	}
	buf.resize(nbytes);

	std::cout << buf << std::endl;
	return CLIENTDATASUCCESS;
	// parsing function here:
};

static bool checkServ(ServerSocket *sockets, int fd) {
	for (int j = 0; j < sockets->getSocketCount(); ++j) {
			if (fd == sockets->getSocketFd(j)) {
				return true;
			}
		}
	return false;
}

int incomingConnection(ServerSocket *sockets, std::vector<pollfd> *fds, Config *config, char **env) {
	// testing
	(void)config;
	(void)env;

	// loop through socket fd's.
	for (int i = 0; i < sockets->getTotalSocketCount(); ++i) {
		if ((*fds)[i].revents & (POLLIN | POLLHUP)) {
			if (checkServ(sockets, (*fds)[i].fd)) {
				handleConnection(sockets, fds, (*fds)[i].fd);
			} else {
				switch(handleClientData((*fds)[i].fd, config, env))
				{
					//testing
					case CLIENTDATASUCCESS:
				//close((*fds)[i].fd);
				//removeFromPollfd(fds, (*fds)[i].fd, sockets);
				break;
					case HUNGUP:
				close((*fds)[i].fd);
				removeFromPollfd(fds, (*fds)[i].fd, sockets);
				break;
					case RECVERROR:
                // sent error, close FD, remove from list
				close((*fds)[i].fd);
				removeFromPollfd(fds, (*fds)[i].fd, sockets);
				break;
					default:
				std::cout << "handleClientData: default: how did you get here?" << std::endl;
				}
			}
		}
	}

	return 0;
};