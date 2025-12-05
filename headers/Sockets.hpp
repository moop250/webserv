#ifndef SOCKET_FUNCS_HPP
# define SOCKET_FUNCS_HPP

# include "SocketClass.hpp"
# include "Connection.hpp"
# include <poll.h>

# define SEND_CHUNK 4096

enum connectionReturns {
	CONNECTIONSUCCESS = 0,
	ACCEPTERROR_NONFATAL = -1,
	ACCEPTERROR_FATAL = -2,
	NOCONNCECTION = -3
};

enum recvReturns {
	EXITPARSING = 0,
	HUNGUP = -1,
	RECVERROR = -2,
	PARSE_TYPE_FDERROR = -3
};

enum fdTypes {
	SERVER,
	CLIENT,
	SYS_FD_IN,
	SYS_FD_OUT,
	CGI_FD_IN,
	CGI_FD_OUT
};

enum fdStatus {
	CLIENTERROR,
	CGIERROR,
	FD_OK,
	TIMEOUT
};

typedef struct s_fdInfo {

	std::vector<pollfd> fds;
	std::map<int, int> fdTypes;
	std::map<int, int> fdStatus;
	std::map<int, time_t> timeout;
	std::map<int, int>	ioFdMap;

} t_fdInfo;

ServerSocket *initalizeServer(Config *serverConfig);
void initPoll(ServerSocket *socket, t_fdInfo *fdInfo);
int incomingConnection(ServerSocket *sockets, t_fdInfo *fdInfo, Config *config, std::map<int, Connection> *connectMap);
void handleTimeout(t_fdInfo *fdInfo);
void removeFromPollfd(t_fdInfo *fdInfo, int fd, ServerSocket *sockets, std::map<int, Connection> *connectMap);

#endif // SOCKET_FUNCS_HPP