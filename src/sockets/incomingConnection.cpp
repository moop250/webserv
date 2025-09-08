#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/Connection.hpp"
#include "../../headers/request_handler.hpp"
#include <cerrno>
#include <climits>
#include <cstring>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <limits>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

static void addToPollfd(std::vector<pollfd> *fds, int newFD, ServerSocket *sockets, std::map<int, Connection> *connectMap) {
	pollfd newPollFD;
	
	newPollFD.fd = newFD;
	newPollFD.events = POLLIN;
	newPollFD.revents = 0;
	fds->push_back(newPollFD);
	
	Connection	newConnection;
	connectMap->insert(std::make_pair(newFD, newConnection));

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

static void setPOLLIN(int fd, std::vector<pollfd> *fds) {
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			it->events = POLLIN;
			break;
		}
	}
}

static void setPOLLOUT(int fd, std::vector<pollfd> *fds) {
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			it->events = POLLOUT;
			break;
		}
	}
}

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

		connect->buffer.append(buf);
		code = parse_request(*connect, *conf, env);
	}

	switch (code) {
		case MAKING_RESPONSE:
			connect->setState(MAKING_RESPONSE);
			return CLIENTDATASUCCESS;
		default:
			return CLOSEFD;
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
				std::cout << YELLOW << "POLLIN: socket " << fd << " closed" << RESET << std::endl;
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				return -1;
			case CLIENTDATASUCCESS:
				setPOLLOUT(fd, fds);
				return 1;
			case HUNGUP:
				std::cout << YELLOW << "Recv: socket " << fd << " hung up" << RESET << std::endl;
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				return -1;
			case RECVERROR:
				close(fd);
				removeFromPollfd(fds, fd, sockets, connectMap);
				return -1;
		}
	}
	return 0;
}

static int handlePOLLOUT(int fd, std::map<int, Connection> *connectMap) {
	Connection &connect = connectMap->at(fd);
	Response resp =  connect.getResponse();
	std::string out = resp.getResponseComplete();
	size_t remainingBytes = out.size(), offset = 0;
	const char *buf = out.c_str();

	if (connect.getOffset() > 0) {
		buf += connect.getOffset();
		offset = connect.getOffset();
		remainingBytes -= offset;
	}

	while (remainingBytes > 0) {
		ssize_t status = send(fd, buf, remainingBytes, MSG_DONTWAIT);
		if (status < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK){
				connect.setOffset(offset); 
				return 1;
			} else if (errno == EFAULT || errno == EINVAL) {
				throw std::runtime_error("Send error: " + std::string(strerror(errno)));
			} else {
				return 2;
			}
		}
		offset += status;
		buf += status;
		remainingBytes -= status;
	}

	connect.setOffset(-2);
	return 0;
}

int incomingConnection(ServerSocket *sockets, std::vector<pollfd> *fds, Config *config, char **env, std::map<int, Connection> *connectMap) {
	for (int i = 0; i < sockets->getTotalSocketCount(); ++i) {
		if ((*fds)[i].revents & POLLIN) {
			if (handlePOLLIN((*fds)[i].fd, sockets, fds, connectMap, config, env) <= 0) {
				continue;
			}
		}
		if ((*fds)[i].revents & POLLHUP) {
			close((*fds)[i].fd);
			removeFromPollfd(fds, (*fds)[i].fd, sockets, connectMap);
			std::cout << YELLOW << "poll: socket " << (*fds)[i].fd << " hung up" << RESET << std::endl;
			continue;
		}
		if ((*fds)[i].revents & POLLOUT) {		
			switch (handlePOLLOUT((*fds)[i].fd, connectMap)) {
				case 0:
					connectMap->at((*fds)[i].fd).clear();
					connectMap->at((*fds)[i].fd).setState(WAITING_REQUEST);
					setPOLLIN((*fds)[i].fd, fds);
					continue ;
				case 1:
					continue ;
				case 2:
					std::cout << YELLOW << "POLLOUT: non fatal error on socket " << (*fds)[i].fd << "... closing" << RESET << std::endl;
					close((*fds)[i].fd);
					removeFromPollfd(fds, (*fds)[i].fd, sockets, connectMap);
			}
		}
	}

	return 0;
};