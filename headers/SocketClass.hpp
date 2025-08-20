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
		std::vector<int>	serverSocketFd_;
		std::vector<int>	clientSocketFd_;


	public:
		ServerSocket();
		~ServerSocket();

		int		getSocketFd(int pos);
		int		getSocketCount(void);
		int		getTotalSocketCount(void);
		void	initializeNewSocket(std::string combo);

};

#endif