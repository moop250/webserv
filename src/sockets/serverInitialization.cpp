#include "../../headers/Colors.hpp"
#include "../../headers/serverInitialization.hpp"
#include "../../headers/StdLibs.hpp"
#include "../../headers/SocketClass.hpp"
#include <cstddef>
#include <exception>
#include <string>
#include <vector>

static bool compareConfigs(std::string currentPair, std::vector<std::string> allPairs) {
	for (int i = allPairs.size(); i > 0; --i) {
		if (allPairs.at(i) == currentPair)
			return false;
	}
	return true;
}

static std::vector<std::string> sanitizeConfig(Config serverConfig) {
	std::vector<std::string> out;

	if (serverConfig.getNbServers() <= 0)
		throw std::runtime_error("sanitizeConfig error: No Servers in config file");
	for (int i = serverConfig.getNbServers(); i > 0; --i) {
		t_ServerData serv = serverConfig.getServerData(i);
		std::string tmp;
		// appends the string to be "host|port"
		tmp = serv.listeners.at(0) + "|" + serv.listeners.at(1);
		if (compareConfigs(tmp, out))
			out.push_back(tmp);
	}
	return out;
};

ServerSocket initalizeServer(Config serverConfig) {
	std::vector<std::string> uniqueComboList;
	ServerSocket socket;

	uniqueComboList = sanitizeConfig(serverConfig);

	for (int i = uniqueComboList.size(); i >= 0; --i) {
		socket.initializeNewSocket_(uniqueComboList.at(i));
	}
	return socket;
};