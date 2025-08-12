#include "../../headers/Colors.hpp"
#include "../../headers/StdLibs.hpp"
#include "../../headers/SocketClass.hpp"
#include <cstddef>
#include <exception>
#include <vector>

static std::vector<std::string> sanitizeConfig(Config serverConfig) {


};

std::vector<ServerSocket *> initalizeServer(Config serverConfig) {
	std::vector<std::string> uniqueComboList;
	std::vector<ServerSocket *> sockets;
	ServerSocket *tmp;

	uniqueComboList = sanitizeConfig(serverConfig);

	for (size_t i = uniqueComboList.size(); i >= 0; --i) {
		tmp = new ServerSocket(uniqueComboList.at(i));
		sockets.push_back(tmp);
	}

	return sockets;
};