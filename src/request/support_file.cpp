/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_file.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:14:22 by hoannguy          #+#    #+#             */
/*   Updated: 2025/08/18 20:37:52 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "support_file.hpp"

const std::set<std::string> supportCgiExtension = {
	".cgi",
	".java",
	".js"
};

const std::map<std::string, std::string> supportDataType = {
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
	{".html", "text/html"},
    {".css", "text/css"},
    {".txt", "text/plain"},
	{".pdf", "application/pdf"},
	{".js", "application/javascript"},
};

bool isCGI(std::string& extension) {
	if (supportCgiExtension.count(extension) == 1)
		return true;
	return false;
}