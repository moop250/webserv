#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include "sys/socket.h"
#include <netdb.h>
#include <string>
#include <vector>

# define BACKLOG 20

class ServerSocket {

	public:
		ServerSocket();
		~ServerSocket();

		int		getSocketFd(int pos);
		void	initializeNewSocket_(std::string combo);

	private:
		std::vector<int>	socketFd_;
};

#endif