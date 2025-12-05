/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:44:39 by hoannguy          #+#    #+#             */
/*   Updated: 2025/10/09 11:52:09 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

void send_error(std::string message) {
	std::cout << "Content-Type: text/plain\r\n";
	std::cout << "Status: 200 OK\r\n";
	std::cout << "Content-Length: " << message.size() << "\r\n\r\n";
	std::cout << message;
}

void get_input(std::string& input) {
	std::string tmp;

	tmp.append(std::getenv("QUERY_STRING"));
	std::string::size_type equal_pos;
	equal_pos = tmp.find("=");
	if (equal_pos != std::string::npos) {
		tmp = tmp.substr(equal_pos + 1);
	}
	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i] == '%' && i + 2 < tmp.size()) {
			char h1 = tmp[i + 1];
			char h2 = tmp[i + 2];
			if ((h1 == '2' && h2 == '0')) input.push_back(' ');
			else if ((h1 == '2' && h2 == '8')) input.push_back('(');
			else if ((h1 == '2' && h2 == '9')) input.push_back(')');
			else if ((h1 == '2' && h2 == 'A') || (h1 == '2' && h2 == 'a')) input.push_back('*');
			else if ((h1 == '2' && h2 == 'B') || (h1 == '2' && h2 == 'b')) input.push_back('+');
			else if ((h1 == '2' && h2 == 'F') || (h1 == '2' && h2 == 'f')) input.push_back('/');
			else {
				input.push_back('%');
				continue;
			}
			i += 2;
			continue;
		}
		if (tmp[i] == '+') {
			tmp[i] = ' ';
		}
		input.push_back(tmp[i]);
	}
}

int main(void) {
	RPN			rpn;
	std::string	input;
	char		*content_len;
	int			len;

	std::string method(std::getenv("REQUEST_METHOD"));
	if (method == "GET" || method == "Get" || method == "get") {
		get_input(input);
	} else if (method == "POST" || method == "Post" || method == "post") {
		content_len = std::getenv("CONTENT_LENGTH");
		if (!content_len) {
			send_error("Error: Missing Content-Length");
			return -1;
		}
		len = std::atoi(content_len);
		input.reserve(len);
		for (int i = 0; i < len; ++i) {
			char c;
			std::cin.get(c);
			input += c;
		}
		while (!input.empty() && (input[input.size() - 1] == '\n' || input[input.size() - 1] == '\r'))
			input.erase(input.size() - 1);
	} else {
		send_error("Error: Bad request method");
		return -1;
	}
	if (rpn.doTheMagic(input) == -1) {
		send_error("Error: Bad input");
		return -1;
	}
	return 0;
}
