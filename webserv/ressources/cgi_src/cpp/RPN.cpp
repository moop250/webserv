/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:15:22 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/13 17:15:22 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() {
}
RPN::RPN(const RPN& copy) {
	this->list = copy.list;
}
RPN& RPN::operator =(const RPN& assign) {
	if (this != &assign)
		this->list = assign.list;
	return *this;
}
RPN::~RPN() {
}

int RPN::first_second(std::string& input) {
	size_t		space_pos;
	std::string	item;
	int			first;
	int			second;

	try {
		space_pos = input.find(" ");
		item = input.substr(0, space_pos);
		first = str_to_int(item);
		this->list.push(first);
		if (space_pos == std::string::npos) {
			print_result(input, item);
			return -2;
		}
		input.erase(0, space_pos + 1);
		space_pos = input.find(" ");
		if (space_pos == std::string::npos)
			return -1;
		item = input.substr(0, space_pos);
		second = str_to_int(item);
		this->list.push(second);
		input.erase(0, space_pos + 1);
	} catch (std::exception& e) {
		return -1;
	}
	return 0;
}

int RPN::operation(const std::string& item) {
	long	first;
	long	second;

	if (this->list.size() < 2)
		return -1;
	second = this->list.top();
	this->list.pop();
	first = this->list.top();
	this->list.pop();
	if (item == "+")
		this->list.push(first + second);
	if (item == "-")
		this->list.push(first - second);
	if (item == "*")
		this->list.push(first * second);
	if (item == "/") {
		if (second == 0)
			return -1;
		this->list.push(first / second);
	}
	if (this->list.top() > 2147483647 || this->list.top() < -2147483648)
		return -1;
	return 0;
}

int RPN::doTheMagic(std::string& input) {
	size_t		space_pos;
	std::string	item;
	int			code;
	std::string	result;
	std::string tmp;
	
	tmp = input;
	code = first_second(input);
	if (code == -1) {
		return -1;
	}
	if (code == -2) {
		return 0;
	}
	while (true) {
		space_pos = input.find(" ");
		if (space_pos == std::string::npos) {
			item = input.substr(0);
			if (isOperator(item) == false || this->operation(item) == -1) {
				return -1;
			}
			result = int_to_str(this->list.top());
			print_result(tmp, result);
			return 0;
		}
		item = input.substr(0, space_pos);
		input.erase(0, space_pos + 1);
		if (isOperator(item) == true) {
			if (operation(item) == -1) {
				return -1;
			}
			continue;
		}
		try {
			int number = str_to_int(item);
			this->list.push(number);
		} catch (std::exception& e) {
			return -1;
		}
	}
	return 0;
}

bool isOperator(const std::string& input) {
	if (input == "+"
	|| input == "-"
	|| input == "*"
	|| input == "/")
		return true;
	return false;
}

long str_to_int(std::string& input) {
	std::stringstream	s(input);
	long				n;

	s >> n;
	if (s.fail() || !s.eof() || n > 2147483647 || n < -2147483648) {
		throw std::runtime_error("Error");
	}
	return n;
}

void print_result(std::string& input, std::string& result) {
	std::cout << "Content-Type: text/html\r\n";
	std::cout << "Status: 200 OK\r\n";
	std::cout << "Content-Length: " << (128 + input.size() + result.size()) << "\r\n\r\n";
	std::cout << "<!DOCTYPE html>\n";
	std::cout << "<html><head><title>Result</title></head><body>\n";
	std::cout << "<h1>Result</h1>\n";
	std::cout << "<p>Before: " << input << "</p>\n";
	std::cout << "<p>After: " << result << "</p>]\n";
	std::cout << "</body></html>";
}

std::string int_to_str(int number) {
	std::stringstream ss;
	std::string str;

	ss << number;
	str = ss.str();
	return str;
}