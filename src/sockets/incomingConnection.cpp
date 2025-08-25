#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include <climits>
#include <poll.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <limits>
#include <unistd.h>

static void addToPollfd(struct pollfd **fds, int newFD, ServerSocket *sockets) {
	int pos = sockets->getSocketCount();
	*fds = (struct pollfd *)realloc(*fds, sizeof(**fds) * (pos + 1));

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

	for (; pos < sockCount - 1; ++pos) {
		(*fds)[pos].fd = (*fds)[pos + 1].fd;
		(*fds)[pos].events = (*fds)[pos + 1].events;
		(*fds)[pos].revents = (*fds)[pos + 1].revents;
	}

	*fds = (struct pollfd *)realloc(*fds, sizeof(**fds) * (sockCount - 1));

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

static int handleClientData(int fd, int port, Config *config, char **env) {

	std::string buf(LONG_MAX - 3000, '\0');

	size_t nbytes = recv(fd, &buf[0], LONG_MAX - 3000, 0);

	if (nbytes <= 0) {
		if (nbytes == 0) {
			std::cout << "Recv: socket " << fd << " hung up" << std::endl;
			return HUNGUP;
		} else {
			return RECVERROR;
		}
	}
	buf.resize(nbytes);

	// parsing function here:
};

static int checkServ(ServerSocket *sockets, int fd) {
	for (size_t j = 0; j < sockets->getSocketCount(); ++j) {
			if (fd == sockets->getSocketFd(j)) {
				return j;
			}
		}
	return -1;
}

int incomingConnection(ServerSocket *sockets, struct pollfd **fds, Config *config, char **env) {

	// loop through socket fd's.
	for (size_t i = 0; i < sockets->getTotalSocketCount(); ++i) {
		int port = -1;
		if ((*fds)[i].revents & (POLLIN | POLLHUP)) {
			if ((port = checkServ(sockets, (*fds)[i].fd)) > 0) {
				handleConnection(sockets, fds, (*fds)[i].fd);
			} else {
				switch(handleClientData((*fds)[i].fd, port, config, env))
				{
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