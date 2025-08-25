#include "../../headers/Colors.hpp"
#include "../../headers/Sockets.hpp"
#include "../../headers/StdLibs.hpp"
#include "../../headers/SocketClass.hpp"
#include <new>
#include <poll.h>
#include <sys/poll.h>

static bool compareConfigs(std::string currentPair, std::vector<std::string> allPairs) {
	if (allPairs.empty())
		return true;
	
	for (size_t i = 0; i < allPairs.size(); ++i) {
		if (allPairs.at(i) == currentPair)
			return false;
	}

	return true;
}

static std::vector<std::string> sanitizeConfig(Config *serverConfig) {
	std::vector<std::string> out;

	if (serverConfig->getNbServers() <= 0)
		throw std::runtime_error("sanitizeConfig error: No Servers in config file");

	for (int i = 0; i < serverConfig->getNbServers(); ++i) {
		t_ServerData serv = serverConfig->getServerData(i);
		std::string tmp;
		tmp = serv.host + "|" + serv.port;

		if (compareConfigs(tmp, out))
			out.push_back(tmp);
	}

	return out;
};

ServerSocket initalizeServer(Config *serverConfig) {
	std::vector<std::string> uniqueComboList;
	ServerSocket socket;

	uniqueComboList = sanitizeConfig(serverConfig);

	for (size_t i = 0; i < uniqueComboList.size(); i++) {
		socket.initializeNewSocket(uniqueComboList.at(i));
	}

	return socket;
};

struct pollfd *initPoll(ServerSocket *socket) {
	struct pollfd *out = new struct pollfd[socket->getTotalSocketCount()];

	for (int i = 0; i < socket->getSocketCount(); ++i) {
		out[i].fd = socket->getSocketFd(i);
		out[i].events = POLLIN;
	}

	return out;
};