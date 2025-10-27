#include "../../headers/GenFD.hpp"

int handleFDIn(t_fdInfo *fdInfo, int fd, Connection *connect) {
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
		return -1;
	}

	close(fd);	
	return 0;
}

int handleFDOut(t_fdInfo *fdInfo, int fd, Connection *connect) {
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