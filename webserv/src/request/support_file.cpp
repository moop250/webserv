/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_file.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:14:22 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/24 11:56:27 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "support_file.hpp"

std::set<std::string> listCGI() {
	std::set<std::string> supported;
	// check with config file to change implementation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	supported.insert(".cpp");
	supported.insert(".java");
	supported.insert(".js");
	supported.insert(".php");
	supported.insert(".py");
	return supported;
}

std::map<std::string, std::string> listExtension() {
	std::map<std::string, std::string> format;
	format[".png"] = "image/png";
	format[".jpg"] = "image/jpeg";
	format[".jpeg"] = "image/jpeg";
	format[".html"] = "text/html";
	format[".css"] = "text/css";
	format[".txt"] = "text/plain";
	format[".js"] = "application/javascript";
	return format;
}

std::map<std::string, std::string> listDataType() {
	std::map<std::string, std::string> format;
	format["image/png"] = ".png";
	format["image/jpeg"] = ".jpg";
	format["image/jpeg"] = ".jpeg";
	format["text/html"] = ".html";
	format["text/css"] = ".css";
	format["text/plain"] = ".txt";
	format["application/javascript"] = ".js";
	return format;
}

std::set<std::string> supportCgiExtension = listCGI();
std::map<std::string, std::string> supportExtension = listExtension();
std::map<std::string, std::string> supportDataType = listDataType();


bool isCGI(std::string& extension) {
	if (supportCgiExtension.count(extension) == 1)
		return true;
	return false;
}

std::string getMIMEType(const std::string extension) {
	std::map<std::string, std::string>::const_iterator it = supportExtension.find(extension);
	if (it != supportExtension.end())
		return it->second;
	return "application/octet-stream";
}

std::string getExtension(const std::string& type) {
	std::map<std::string, std::string>::const_iterator it = supportDataType.find(type);
	if (it != supportDataType.end())
		return it->second;
	return ".bin";
}