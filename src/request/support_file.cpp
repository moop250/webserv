/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_file.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:14:22 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/03 22:25:54 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "support_file.hpp"

std::set<std::string> listCGI() {
	std::set<std::string> supported;
	// check with config file to change implementation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	supported.insert(".cgi");
	supported.insert(".java");
	supported.insert(".js");
	supported.insert(".php");
	supported.insert(".py");
	return supported;
}

std::map<std::string, std::string> listFormat() {
	std::map<std::string, std::string> format;
	format.insert(std::make_pair(".png", "image/png"));
	format.insert(std::make_pair(".jpg", "image/jpeg"));
	format.insert(std::make_pair(".jpeg", "image/jpeg"));
	format.insert(std::make_pair(".html", "text/html"));
	format.insert(std::make_pair(".css", "text/css"));
	format.insert(std::make_pair(".txt", "text/plain"));
	format.insert(std::make_pair(".pdf", "application/pdf"));
	format.insert(std::make_pair(".js", "application/javascript"));
	return format;
}

std::set<std::string> supportCgiExtension = listCGI();
std::map<std::string, std::string> supportDataType = listFormat();


bool isCGI(std::string& extension) {
	if (supportCgiExtension.count(extension) == 1)
		return true;
	return false;
}
std::string getMIMEType(const std::string type) {
	std::map<std::string, std::string>::const_iterator it = supportDataType.find(type);
	if (it != supportDataType.end())
		return it->second;
	return "application/octet-stream";
}
