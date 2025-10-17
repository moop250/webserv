#include "../../headers/SocketClass.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/Connection.hpp"
#include "../../headers/request_handler.hpp"
#include <string>

int handleFDIn(t_fdInfo *fdInfo, int fd, std::string *output) {
	char	buffer[4096];
	ssize_t	n = 0;
		
	n = read(fd, buffer, sizeof(buffer));
	if (n > 0) {
		output->append(buffer, n);
		return 1;
	}
	if (n == 0) {
		return 0;
	}
	if (n < 0) {
		close(fd);
		return -1;
	}

	return 0;
}

int handleFDOut(t_fdInfo *fdInfo, int fd) {

	return 0;
}