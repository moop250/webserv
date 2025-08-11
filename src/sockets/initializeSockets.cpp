#include "../../headers/StdLibs.hpp"
#include "../../headers/Config.hpp"
#include <string>
#include <vector>

std::vector<std::string> sanitizeConfig(Config serverConfig) {


};

int initializeSockets(Config serverConfig) {
	
	std::vector<std::string> uniqueComboList;

	// Sanitize the config and return a list of every unique host and port combination

	uniqueComboList = sanitizeConfig(serverConfig);

	for (size_t i = uniqueComboList.size(); i >= 0; --i) {
		uniqueComboList.at(i);
		// Create a socket for the current unique host and port combinatuion

		// Bind the combo to a socket
		// Start listenting to the socket

		// save file descriptor to a class or something to be returned
	}

	// return status code
};