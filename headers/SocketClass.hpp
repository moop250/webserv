#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <vector>

# define BACKLOG 20

class ServerSocket {

	private:
		std::vector<int>	socketFd_;

	public:
		ServerSocket();
		~ServerSocket();

		int		getSocketFd(int pos);
		int		getSocketCount(void);
		void	initializeNewSocket_(std::string combo);

};

#endif