#include "../../headers/SocketClass.hpp"
#include <poll.h>
#include <sys/poll.h>

int handleConnection(ServerSocket sockets, struct pollfd **fds, int fd) {

    // accept new client connction

    // add new fd to sockets and struct
};

int handleClientData(int fd, int port) {

    // create a buffer

    // recv the data

    // send data to be parsed
};

static int checkServ(ServerSocket sockets, int fd) {
    for (size_t j = 0; j < sockets.getSocketCount(); ++j) {
            if (fd == sockets.getSocketFd(j)) {
                return j;
            }
        }
    return -1;
}

int incomingConnection(ServerSocket sockets, struct pollfd **fds) {

    // loop through socket fd's.
    for (size_t i = 0; i < sockets.getTotalSocketCount(); ++i) {
        int port = -1;
        if ((*fds)[i].revents & (POLLIN | POLLHUP)) {
            if ((port = checkServ(sockets, (*fds)[i].fd)) > 0) {
                handleConnection(sockets, fds, (*fds)[i].fd);
            } else {
                handleClientData((*fds)[i].fd, port);
            }
        }
    }

    return 0;
};