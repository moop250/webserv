#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include <vector>

class ServerSocket {

	public:
		ServerSocket(std::string);
		~ServerSocket();

		int	getFd();

	private:
		void	initializeSocket_(std::string combo);
		int		server_fd_;
};

#endif