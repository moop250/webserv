#include "../../headers/SocketClass.hpp"

int handleConnection() {

    // accept new client connction

    // add new fd to sockets and struct
};

int handleClientData() {

    // create a buffer

    // recv the data

    // send data to be parsed
};

int incomingConnection(ServerSocket sockets) {

    // loop through socket fd's.

    // if the socket has data to read, or if there is a new connection

    // if new connection, handle new connection

    // if data available on existing connection, recv data and launch parsing
    return 0;
};