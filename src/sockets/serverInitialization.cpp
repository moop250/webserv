#include "../../headers/Colors.hpp"
#include "../../headers/serverInitialization.hpp"
#include "../../headers/StdLibs.hpp"
#include "../../headers/SocketClass.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

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
		// appends the string to be "host|port"
		tmp = serv.host + "|" + serv.port;
		std::cout << tmp << '\n';
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
		socket.initializeNewSocket_(uniqueComboList.at(i));
	}
	return socket;
};