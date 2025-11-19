#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/Connection.hpp"
#include "../../headers/request_handler.hpp"
#include "../../headers/GenFD.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <map>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <limits>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

void addToPollfd(t_fdInfo *fdInfo, int newFD, ServerSocket *sockets, std::map<int, Connection> *connectMap, int fdType) {
	pollfd newPollFD;
	std::map<int, int> *status = &fdInfo->fdStatus;
	std::vector<pollfd> *fds = &fdInfo->fds;

	status->insert(std::make_pair(newFD, FD_OK));
	int flags = fcntl(newFD, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << RED << "[ERROR] : " << WHITE << "addToPollfd: Failed to get socket flags for fd: " << newFD << " as type: " << fdType << " cancelling" << std::endl;
		if (fdType == CGI) {
			status->at(newFD) = CGIERROR;
		} else {
			status->at(newFD) = CLIENTERROR;
		}
	}

	if (fcntl(newFD, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << RED << "[ERROR] : " << WHITE << "addToPollfd: Failed to set non-blocking mode for fd: " << newFD << " as type: " << fdType << " cancelling" << std::endl;
		if (fdType == CGI) {
			status->at(newFD) = CGIERROR;
		} else {
			status->at(newFD) = CLIENTERROR;
		}
	}
	
	newPollFD.fd = newFD;
	newPollFD.events = POLLIN;
	newPollFD.revents = 0;
	fds->push_back(newPollFD);
	
	Connection	newConnection;
	newConnection.setState(READING_METHOD);
	connectMap->insert(std::make_pair(newFD, newConnection));
	fdInfo->fdTypes.insert(std::make_pair(newFD, fdType));
	sockets->incrementClientCount();
}

void removeFromPollfd(t_fdInfo *fdInfo, int fd, ServerSocket *sockets, std::map<int, Connection> *connectMap) {
	std::vector<pollfd> *fds = &fdInfo->fds;
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			fds->erase(it);
			break;
		}
	}
	if (fcntl(fd, F_GETFD) != -1) {
		close(fd);
	}
	connectMap->erase(fd);
	fdInfo->fdTypes.erase(fd);
	fdInfo->fdStatus.erase(fd);
	if (fdInfo->timeout.count(fd) > 0) {
		fdInfo->timeout.erase(fd);
	}

	sockets->decrementClientCount();
}

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

void handleTimeout(s_fdInfo *fdInfo) {
	time_t current_time = time(NULL);
	
	for (std::map<int, time_t>::iterator it = fdInfo->timeout.begin(); it != fdInfo->timeout.end(); ++it) {
		if (difftime(current_time, it->second) >= 300) {
			fdInfo->fdStatus[it->first] = TIMEOUT;
			setPOLLOUT(it->first, &fdInfo->fds);
		}
	}
}

static int handleConnection(ServerSocket *sockets, t_fdInfo *fdInfo, int fd, std::map<int, Connection> *connectMap) {
	struct sockaddr_storage newRemote;
	socklen_t               addrLen;
	int remoteFD;
	std::string remoteIP;

	addrLen = sizeof newRemote;
	remoteFD = accept(fd, (struct sockaddr *)&newRemote,&addrLen);
	if (remoteFD == -1) {
		if (errno == EINTR) {
			std::cout << YELLOW << "[WARNING]	: " << WHITE << "accept() was interrupted by a signel. Retrying next loop..." << RESET << std::endl;
			return ACCEPTERROR_NONFATAL;
		} else if (errno == EMFILE || errno == ENFILE) {
			std::cout << RED << "[ERROR]	: " << WHITE << "accept() failed: " << strerror(errno) << std::endl;
			return ACCEPTERROR_NONFATAL;
		} else if (errno == ENOMEM) {
			std::cout << RED << "[ERROR]	: " << WHITE << "accept() failed: " << strerror(errno) << std::endl;
			return ACCEPTERROR_FATAL;
		} else {
			std::cout << RED << "[ERROR]	: " << WHITE << "accept() failed: " << strerror(errno) << std::endl;
			return ACCEPTERROR_FATAL;
		}
	}
	addToPollfd(fdInfo, remoteFD, sockets, connectMap, CLIENT);

	t_connectionAddrInfo tmp = sockets->getServerAddrInfo(fd);
	connectMap->at(remoteFD).setIP(tmp.address);
	connectMap->at(remoteFD).setPort(tmp.port);
	if (fdInfo->fdStatus.at(remoteFD) == CLIENTERROR)
		setPOLLOUT(remoteFD, &fdInfo->fds);
	return remoteFD;
};

static int handleClientData(t_fdInfo *fdInfo, int fd, std::map<int, Connection> *connectMap, Config *conf) {
	Connection *connect = &connectMap->at(fd);
	std::string buf(8192, '\0');

	int nbytes = recv(fd, &buf[0], buf.size(), 0);
	if (nbytes < 0) {
		return RECVERROR;
	} else if (nbytes == 0)
		return HUNGUP;

	buf.resize(nbytes);

	connect->buffer.append(buf);
	int state = parse_request(*connect, *conf);
	if (state == CONTINUE_READ)
		return CONTINUE_READ;
	if (state == -1)
		return EXITPARSING;
	parse_type_fd(*connect);

	if (connect->getFDIN() > 0) {
			addToGenFD(fdInfo, connect->getFDIN(), fd, SYS_FD_IN);
		}
	if (connect->getFDOUT() > 0) {
		addToGenFD(fdInfo, connect->getFDOUT(), fd, SYS_FD_OUT);
	}
	
	if (connect->getFDIN() <= 0 && connect->getFDOUT() <= 0) {
		connect->setState(MAKING_RESPONSE);
	}

	return EXITPARSING;
};

static int handlePOLLIN(int fd, ServerSocket *sockets, t_fdInfo *fdInfo, std::map<int, Connection> *connectMap, Config *conf) {
	switch (fdInfo->fdTypes.at(fd)) {
		case SERVER: {
			int tmp = handleConnection(sockets, fdInfo, fd, connectMap);
			switch (tmp)
			{
				case NOCONNCECTION:
					return -1;
				case ACCEPTERROR_NONFATAL:
					return -2;
				case ACCEPTERROR_FATAL:
					throw std::runtime_error("handlePOLLIN Accept error");
				default:
					std::cout <<   CYAN << "[INFO]		: " << RESET << "Accept: New connection on socket: " << tmp << RESET << std::endl;
			}
			break ;
		} case CLIENT: {
			if (fdInfo->timeout.count(fd) > 0) {
				fdInfo->timeout.erase(fd);
			}
			switch(handleClientData(fdInfo, fd, connectMap, conf))
			{
				case EXITPARSING:
					setPOLLOUT(fd, &fdInfo->fds);
					return 1;
				case HUNGUP:
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "Recv: socket " << fd << " hung up" << RESET << std::endl;
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					return -1;
				case RECVERROR:
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					return -1;
				case PARSE_TYPE_FDERROR:
					setPOLLOUT(fd, &fdInfo->fds);
					fdInfo->fdStatus.at(fd) = CLIENTERROR;
					connectMap->at(fd).setState(SENDING_RESPONSE);
					return -1;
				case CONTINUE_READ:
					return 1;
			}
			break ;
		} default:
			std::cout << RED << "[ERROR]	: " << WHITE << "Unknown POLLIN type" << RESET << std::endl;
	}
	return 0;
}

static int handlePOLLOUT(int fd, std::map<int, Connection> *connectMap, t_fdInfo *fdInfo) {
	Connection &connect = connectMap->at(fd);
	if (connect.getState() == MAKING_RESPONSE) {
		handle_request_remake(connect);
	}

	if (fdInfo->fdStatus.at(fd) == CLIENTERROR && connect.getResponse().getCode() != 500) {
		connect.clear();
		connect.setClose(true);
		error_response(connect, 500);
	} else if (fdInfo->fdStatus.at(fd) == TIMEOUT) {
		return 6;
	}

	Response resp =  connect.getResponse();
	std::string out = resp.getResponseComplete();
	ssize_t offset = connect.getOffset(), remainingBytes = (out.size() - offset);

	ssize_t chunk = (remainingBytes < SEND_CHUNK) ? remainingBytes : SEND_CHUNK;
	ssize_t status = send(fd, out.c_str() + offset, chunk, 0);
	if (status < 0)
		return 2;
	if (status == 0)
		return 5;

	remainingBytes -= status;
	if (remainingBytes > 0) {
		connect.setOffset(offset + status);
		return 1;
	} 

	if (fdInfo->fdStatus.at(fd) == CLIENTERROR) {
		return 2;
	}

	connect.setOffset(0);
	if (connect.getClose())
		return 4;
	else if (connect.getRequest().getKeepAlive() == "keep-alive")
		return 4;
	// [INFO] replace the 3 with a 4 to disable keep alive
	// 400 Bad request with post happens after 0 is returned
	return 0;
}

int incomingConnection(ServerSocket *sockets, t_fdInfo *fdInfo, Config *config, std::map<int, Connection> *connectMap) {
	for (size_t i = 0; i < fdInfo->fds.size(); ++i) {
		int fd = fdInfo->fds.at(i).fd;
		// std::cout << "current fd: " << fd << std::endl;
		
		if (fdInfo->fdTypes.at(fd) == CLIENT) {
			Connection &connect = connectMap->at(fd);
			if (connect.getState() == IO_OPERATION && connect.getRequestType() == CGI) {
				CGI_timeout(connect);
			}
		}

		if (fdInfo->fds.at(i).revents & POLLHUP) {
			if (fdInfo->fdTypes.at(fd) == CLIENT || fdInfo->fdTypes.at(fd) == SERVER) {
				if (connectMap->at(fd).getFDIN() > 0) {
					removeFromGenfd(fdInfo, connectMap->at(fd).getFDIN());
					connectMap->at(fd).setFDIN(-1);
				}
				if (connectMap->at(fd).getFDOUT() > 0) {
					removeFromGenfd(fdInfo, connectMap->at(fd).getFDOUT());
					connectMap->at(fd).setFDOUT(-1);
				}
				removeFromPollfd(fdInfo, fd, sockets, connectMap);
			} else {
				int originFd = fdInfo->ioFdMap.at(fd);
				if (connectMap->at(originFd).getState() == MAKING_RESPONSE || connectMap->at(originFd).getState() == IO_OPERATION) {
					handle_request_remake(connectMap->at(originFd));
				}
				removeFromGenfd(fdInfo, fd);
			}
			std::cout << YELLOW << "poll: socket " << fd << " hung up" << RESET << std::endl;
		}
		else if (fdInfo->fds.at(i).revents & POLLIN) {
			if (fdInfo->fdTypes.at(fd) == SYS_FD_IN || fdInfo->fdTypes.at(fd) == SYS_FD_OUT) {
				int originFD = fdInfo->ioFdMap.at(fd);
				std::cout << CYAN << "[INFO] : Poll in triggered on CGI FD " << fd
						<< " (origin client FD " << originFD << ")" << RESET << std::endl;
			}

			if (fdInfo->fdTypes.at(fd) == SERVER || fdInfo->fdTypes.at(fd) == CLIENT) {
				handlePOLLIN(fd, sockets, fdInfo, connectMap, config);
			} else {
				int fdin = fdInfo->ioFdMap.at(fd);
				handle_request_remake(connectMap->at(fdin));
				if (connectMap->at(fdin).getFDIN() == -1) {
					removeFromGenfd(fdInfo, fd);
				}
			}
			continue;
		}
		else if (fdInfo->fds.at(i).revents & POLLOUT) {
			if (fdInfo->fdTypes.at(fd) == SYS_FD_IN || fdInfo->fdTypes.at(fd) == SYS_FD_OUT) {
				int originFD = fdInfo->ioFdMap.at(fd);
				std::cout << CYAN << "[INFO] : Poll out triggered on CGI FD " << fd
						<< " (origin client FD " << originFD << ")" << RESET << std::endl;
			}

			if (fdInfo->fdTypes.at(fd) != CLIENT && fdInfo->fdTypes.at(fd) != SERVER) {
				int originFd = fdInfo->ioFdMap.at(fd);
				if (connectMap->at(originFd).getState() == MAKING_RESPONSE || connectMap->at(originFd).getState() == IO_OPERATION) {
					handle_request_remake(connectMap->at(originFd));
					if (connectMap->at(originFd).getFDOUT() == -1) {
						removeFromGenfd(fdInfo, fd);
					}
				}
				continue;
			}
			if (connectMap->at(fd).getState() != SENDING_RESPONSE && connectMap->at(fd).getState() != MAKING_RESPONSE) {
				continue;
			}

			if (connectMap->at(fd).getFDIN() > 0) {
				removeFromGenfd(fdInfo, connectMap->at(fd).getFDIN());
				connectMap->at(fd).setFDIN(-1);
			}
			if (connectMap->at(fd).getFDOUT() > 0) {
				removeFromGenfd(fdInfo, connectMap->at(fd).getFDOUT());
				connectMap->at(fd).setFDOUT(-1);
			}

			switch (handlePOLLOUT(fd, connectMap, fdInfo)) {
				case 0:
					connectMap->at(fd).clear();
					setPOLLIN(fd, &fdInfo->fds);
					continue ;
				case 1:
					continue ;
				case 2:
					std::cout << YELLOW << "[WARNING]	: " << RESET << "POLLOUT: non fatal error on socket: " << fd << "... closing" << RESET << std::endl;
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					continue;
				case 3:
					fdInfo->timeout[fd] = time(NULL);
					std::cout <<   CYAN << "[INFO]		: " << RESET << "Keep-alive timer started" << std::endl;
					connectMap->at(fd).clear();
					setPOLLIN(fd, &fdInfo->fds);
					continue;
				case 4:
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "POLLOUT: close flag on socket: " << fd << "... closing" << RESET << std::endl;
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					continue;
				case 5:
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "POLLOUT: socket " << fd << " hung up" << RESET << std::endl;
					continue;
				case 6:
					close(fd);
					removeFromPollfd(fdInfo, fd, sockets, connectMap);
					std::cout <<   CYAN << "[INFO]		: " << RESET << "POLLOUT: socket " << fd << " timed out... closing" << RESET << std::endl;
					continue;
			}
		}
	}

	return 0;
}