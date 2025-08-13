#include "../../headers/Colors.hpp"
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
	for (int i = serverConfig.getServerCount(); i > 0; --i) {
		Serverdata serv = Config.getServer(i);
		std::string tmp;
		tmp.append(serv.host + "|" + serv.port);
		if (compareConfigs(tmp, out))
			out.push_back(tmp);
	}
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