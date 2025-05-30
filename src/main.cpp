/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlibine <hlibine@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:03:20 by hlibine           #+#    #+#             */
/*   Updated: 2025/05/30 16:32:51 by hlibine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <exception>
#include <stdexcept>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

/* Following https://ncona.com/2019/04/building-a-simple-server-with-cpp/ to try and get a better understanding of how sockets work*/

int main(int ac, char** av) {

	(void)ac;
	(void)av;

	int port;

	// pull port data directly from class.
	port = 9999;

	// Loop over this to create enough sockets 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cerr << "failed to create socket on: " << port << std::endl;
		throw std::exception();
	}

	// gonna need to make an array of sockaddr's.
	// one per open port
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port);

	// Loop of bindings
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cerr << "failed to bind to port: " << port << std::endl;
		throw std::exception();
	}

	if (listen(sockfd, 10) < 0) {
		std::cerr << "failed to listen to socket bound to port: " << port << std::endl;
		throw std::exception();
	}

	size_t addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cerr << "failed to failed to grab connection" << std::endl;
		throw std::exception();
	}

	char buffer[BUFSIZ];
	// going to have to use poll() instead of read im pretty sure.
	size_t bytesRead = read(connection, buffer, 100);
	std::cout << buffer;
	(void)bytesRead;

	std::string response = "A pleasure talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	// close in loop
	close(connection);
	close(sockfd);

	return 0;
}