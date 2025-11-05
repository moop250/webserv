#include "../../headers/GenFD.hpp"

int handleFDIn(int fd, Connection *connect) {
	char	buffer[BUFSIZ];
	std::string *output = &connect->iobuffer;
	ssize_t	n = 0;
		
	n = read(fd, buffer, sizeof(buffer));
	if (n > 0) {
		output->append(buffer, n);
		return 1;
	}
	if (n < 0) {
		close(fd);
		error_response(*connect, INTERNAL_ERROR);
		return -1;
	}

	close(fd);	
	return 0;
}

int handleFDOut(int fd, Connection *connect) {
	std::string body = connect->getRequest().getBody();
	size_t offset = connect->getOffset(), remainingBytes = (body.size() - offset);

	ssize_t status, chunk = (remainingBytes < SEND_CHUNK) ? remainingBytes : SEND_CHUNK;
	status = write(fd, body.c_str() + offset, chunk);
	if (status < 0) {
		close(fd);
		error_response(*connect, INTERNAL_ERROR);
		return -2;
	}

	remainingBytes -= status;
	if (remainingBytes > 0) {
		connect->setOffset(offset + status);
		return 1;
	}

	close(fd);
	return 0;
}

void addToGenFD(t_fdInfo *fdInfo, int newFD, int originFD, int fdType) {
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
	if (fdType == CGI_FD_IN || fdType == SYS_FD_IN)
		newPollFD.events = POLLIN;
	else
		newPollFD.events = POLLOUT;
	newPollFD.revents = 0;
	fds->push_back(newPollFD);

	fdInfo->ioFdMap.insert(std::make_pair(newFD, originFD));
	fdInfo->fdTypes.insert(std::make_pair(newFD, fdType));
}

void removeFromGenfd(t_fdInfo *fdInfo, int fd) {
	std::vector<pollfd> *fds = &fdInfo->fds;
	std::vector<pollfd>::iterator it = fds->begin();
	for (; it != fds->end(); ++it) {
		if (it->fd == fd) {
			fds->erase(it);
			break;
		}
	}
	fdInfo->ioFdMap.erase(fd);
	fdInfo->fdTypes.erase(fd);
	fdInfo->fdStatus.erase(fd);
	if (fdInfo->timeout.count(fd) > 0) {
		fdInfo->timeout.erase(fd);
	}
}