#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/Connection.hpp"
#include "../../headers/request_handler.hpp"
#include "../../headers/GenFD.hpp"
#include <cerrno>
#include <climits>
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

void addToPollfd(t_fdInfo *fdInfo, int newFD, std::map<int, Connection> *connectMap, int fdType) {
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
}

void removeFromPollfd(t_fdInfo *fdInfo, int fd, std::map<int, Connection> *connectMap) {
	std::vector<pollfd> *fds = &fdInfo->fds;
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			fds->erase(it);
			break;
		}
	}
	connectMap->erase(fd);
	fdInfo->fdTypes.erase(fd);
	fdInfo->fdStatus.erase(fd);
	if (fdInfo->timeout.count(fd) > 0) {
		fdInfo->timeout.erase(fd);
	}
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
		if (errno == EINTR)
			std::cout << YELLOW << "[WARNING]	: " << WHITE << "accept() was interrupted by a signel. Retrying next loop..." << RESET << std::endl;
		else if (errno == EMFILE || errno == ENFILE) {
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
	addToPollfd(fdInfo, remoteFD, connectMap, CLIENT);

	t_connectionAddrInfo tmp = sockets->getServerAddrInfo(fd);
	connectMap->at(remoteFD).setIP(tmp.address);
	connectMap->at(remoteFD).setPort(tmp.port);
	if (fdInfo->fdStatus.at(remoteFD) == CLIENTERROR)
		setPOLLOUT(remoteFD, &fdInfo->fds);
	return remoteFD;
};

static int handleClientData(int fd, std::map<int, Connection> *connectMap, Config *conf) {
	Connection *connect = &connectMap->at(fd);
		std::string buf(8192, '\0');

		int nbytes = recv(fd, &buf[0], buf.size(), 0);
		if (nbytes < 0) {
			return RECVERROR;
		} else if (nbytes == 0)
			return HUNGUP;

		buf.resize(nbytes);

		connect->buffer.append(buf);
		if (parse_request(*connect, *conf) == CONTINUE_READ)
			return CONTINUE_READ;

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
			switch(handleClientData(fd, connectMap, conf))
			{
				case EXITPARSING:
					setPOLLOUT(fd, &fdInfo->fds);
					return 1;
				case HUNGUP:
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "Recv: socket " << fd << " hung up" << RESET << std::endl;
					close(fd);
					removeFromPollfd(fdInfo, fd, connectMap);
					return -1;
				case RECVERROR:
					close(fd);
					removeFromPollfd(fdInfo, fd, connectMap);
					return -1;
				case CONTINUE_READ:
					return 1;
			}
			break ;
		} case SYS_FD_IN: {
			// handle receiving data from the system
			if (handleFDIn(fd, &connectMap->at(fd)) == 0) {
				// set related client data
				Connection *connection = &connectMap->at(fd);
				connection->getResponse().setBody(connection->iobuffer);
				connection->getResponse().setCode(200);
				connection->getResponse().setCodeMessage("OK");
				connection->getResponse().setHeader("Content-Length", size_to_string(connection->iobuffer.size()));
				connection->getResponse().setHeader("Content-Type", "text/html");
				if (connection->getRequest().getKeepAlive() == "keep-alive")
					connection->getResponse().setHeader("Connection", "keep-alive");
				connection->getResponse().constructResponse();
				connection->setState(SENDING_RESPONSE); 
				removeFromPollfd(fdInfo, fd, connectMap);
				return 1;
			}

			return 1;
		} case CGI_FD_IN: {
			// handle reciving data from the CGI
			if (handleFDIn(fd, &connectMap->at(fd)) == 0) {
				// set info here

				removeFromPollfd(fdInfo, fd, connectMap);
				return 1;
			}

			return 1;
		} default:
			std::cout << RED << "[ERROR]	: " << WHITE << "Unknown POLLIN type" << RESET << std::endl;
	}
	return 0;
}

static int handlePOLLOUT(int fd, std::map<int, Connection> *connectMap, t_fdInfo *fdInfo) {
	Connection &connect = connectMap->at(fd);

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

	connect.setOffset(-2);
	if (connect.getClose())
		return 4;
	else if (connect.getRequest().getKeepAlive() == "keep-alive")
		return 3;
	// 400 Bad request with post happens after 0 is returned
	return 0;
}

int incomingConnection(ServerSocket *sockets, t_fdInfo *fdInfo, Config *config) {
	std::map<int, Connection> *connectMap = &fdInfo->connectMap;
	for (size_t i = 0; i < fdInfo->fds.size(); ++i) {
		int fd = fdInfo->fds.at(i).fd;

		if (fdInfo->fds.at(i).revents & POLLHUP) {
			close(fd);
			removeFromPollfd(fdInfo, fd, connectMap);
			std::cout << YELLOW << "poll: socket " << fd << " hung up" << RESET << std::endl;
		}
		else if (fdInfo->fds.at(i).revents & POLLIN) {
			handlePOLLIN(fd, sockets, fdInfo, connectMap, config);
		}
		else if (fdInfo->fds.at(i).revents & POLLOUT) {
			if (fdInfo->fdTypes.at(fd) == SYS_FD_OUT) {
				if (handleFDOut(fd, &connectMap->at(fd)) == 0) {
					// run any "on successful send" code here
					// Maybe make it a dedicated function

					removeFromPollfd(fdInfo, fd, connectMap);
					continue;
				}

				continue;
			}


			// make sure connection isnt awaiting a cgi connection
			if (connectMap->at(fd).getState() != CONNECTION_LOCK) {
				handle_request(fd, fdInfo, connectMap->at(fd));
			}
		
			if (connectMap->at(fd).getState() != SENDING_RESPONSE) {
				continue;
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
					removeFromPollfd(fdInfo, fd, connectMap);
					break ;
				case 3:
					fdInfo->timeout[fd] = time(NULL);
					std::cout <<   CYAN << "[INFO]		: " << RESET << "Keep-alive timer started" << std::endl;
					connectMap->at(fd).clear();
					setPOLLIN(fd, &fdInfo->fds);
					continue;
				case 4:
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "POLLOUT: close flag on socket: " << fd << "... closing" << RESET << std::endl;
					close(fd);
					removeFromPollfd(fdInfo, fd, connectMap);
					continue;
				case 5:
					close(fd);
					removeFromPollfd(fdInfo, fd, connectMap);
					std::cout << YELLOW << "[WARNING]	: " << WHITE << "POLLOUT: socket " << fd << " hung up" << RESET << std::endl;
					continue;
				case 6:
					close(fd);
					removeFromPollfd(fdInfo, fd, connectMap);
					std::cout <<   CYAN << "[INFO]		: " << RESET << "POLLOUT: socket " << fd << " timed out... closing" << RESET << std::endl;
					continue;
			}
		}
	}

	return 0;
}