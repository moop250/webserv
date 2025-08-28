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

static void addToPollfd(struct pollfd **fds, int newFD, ServerSocket *sockets) {
	int pos = sockets->getSocketCount();
	struct pollfd *newFds = new struct pollfd[pos + 1];

	for (int i = 0; i < pos; ++i) {
		newFds[i] = (*fds)[i];
	}

	delete [] *fds;
	*fds = newFds;

	(*fds)[pos].fd = newFD;
	(*fds)[pos].events = POLLIN;
	(*fds)[pos].revents = 0;

	sockets->incrementClientCount();
}

static void removeFromPollfd(struct pollfd **fds, int fd, ServerSocket *sockets) {
	int pos = 0;
	int sockCount = sockets->getTotalSocketCount();
	while ((*fds)[pos].fd != fd) {
		++pos;
	}

	struct pollfd *newFds = new struct pollfd[sockCount];

	for (int i = 0; i < pos; ++i) {
		newFds[i] = (*fds)[i];
		// testing
		std::cout << "fd " << i << " moved over" << std::endl;
	}

	// testing
	std::cout << "fd " << pos << " not moved over" << std::endl;

	for (int i = pos + 1; i < sockCount; ++i) {
		// testing
		std::cout << "fd " << i << " moved over" << std::endl;
		newFds[i - 1] = (*fds)[i];
	}

	delete [] *fds;
	*fds = newFds;

	sockets->decrementClientCount();
}

static int handleConnection(ServerSocket *sockets, struct pollfd **fds, int fd) {

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

int incomingConnection(ServerSocket *sockets, struct pollfd **fds, Config *config, char **env) {
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