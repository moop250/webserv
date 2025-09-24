/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:44:39 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/24 12:28:56 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

void send_error() {
	std::cout << "Content-Type: text/plain\r\n";
	std::cout << "Status: 200 OK\r\n";
	std::cout << "Content-Length: 11\r\n\r\n";
	std::cout << "Input Error";
}

int main(int ac, char **av, char **env) {
	RPN			rpn;

	if (ac == 1) {
		send_error();
		return -1;
	}
	
	(void)env;
	std::string input(av[1]);
	
	if (rpn.doTheMagic(input) == -1) {
		send_error();
		return -1;
	}
	return 0;
}
