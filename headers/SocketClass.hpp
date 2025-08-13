#ifndef SOCKET_CLASS_HPP
# define SOCKET_CLASS_HPP

#include "Config.hpp"
#include <vector>

typedef struct socketData {

	int server_fd;

} t_socketData;

class ServerSocket {

	public:
		ServerSocket();
		~ServerSocket();

		t_socketData	getSocketData(int pos);
		void 			initializeNewSocket_(std::string combo);

	private:
		std::vector<t_socketData>	socketData_;
};

#endif