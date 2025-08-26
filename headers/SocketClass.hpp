#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <vector>

# define BACKLOG 20

enum connectionReturns {
	ACCEPTERROR = -1,
	CONNECTIONSUCCESS
};

enum recvReturns {
	CLIENTDATASUCCESS = 0,
	HUNGUP = -1,
	RECVERROR = -2
};

class ServerSocket {

	private:
		std::vector<int>	serverSocketFd_;
		std::vector<int>	serverPort_;
		int					clientCount_;


	public:
		ServerSocket();
		~ServerSocket();

		int		getSocketFd(int pos);
		int		getSocketPort(int pos);
		int		getSocketCount(void);
		int		getTotalSocketCount(void);
		void	initializeNewSocket(std::string combo);
		void	incrementClientCount();
		void	decrementClientCount();

};

#endif