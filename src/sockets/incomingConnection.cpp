#include "../../headers/SocketClass.hpp"
#include <poll.h>
#include <sys/poll.h>

int handleConnection(ServerSocket sockets, struct pollfd **fds, int fd) {

    // accept new client connction

    // add new fd to sockets and struct
};

int handleClientData() {

    // create a buffer

    // recv the data

    // send data to be parsed
};

static bool checkServ(ServerSocket sockets, int fd) {
    for (size_t j = 0; j < sockets.getSocketCount(); ++j) {
            if (fd == sockets.getSocketFd(j)) {
                return true;
            }
        }
    return false;
}

int incomingConnection(ServerSocket sockets, struct pollfd **fds) {

    // loop through socket fd's.
    for (size_t i = 0; i < sockets.getTotalSocketCount(); ++i) {
      if ((*fds)[i].revents & (POLLIN | POLLHUP)) {
        if (checkServ(sockets, (*fds)[i].fd)) {
            handleConnection(sockets, fds, (*fds)[i].fd);
        } else {
            handleClientData();
        }
      }
    }

    return 0;
};