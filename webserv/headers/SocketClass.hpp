#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <vector>

# define BACKLOG 20

typedef struct s_connectionAddrInfo {

	std::string	address;
	std::string port;

} t_connectionAddrInfo;

class ServerSocket {

	private:
		std::vector<int>					serverSocketFd_;
		std::vector<int>					serverPort_;
		std::map<int, t_connectionAddrInfo> serverAddrInfo_;

	public:
		ServerSocket();
		~ServerSocket();

		int						getSocketFd(int pos);
		int						getSocketPort(int pos);
		int						getSocketCount(void);
		void					initializeNewSocket(std::string combo);
		t_connectionAddrInfo	getServerAddrInfo(int fd);
		void					setServerAddrInfo(int fd, std::string address, std::string port);
		void					removeServerAddrInfo(int fd);

};

#endif