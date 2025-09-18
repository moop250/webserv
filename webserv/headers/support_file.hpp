/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_file.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 19:54:56 by hoannguy          #+#    #+#             */
/*   Updated: 2025/09/04 21:33:53 by hoannguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUPPORT_FILE_HPP
#define SUPPORT_FILE_HPP
#include <string>
#include <map>
#include <set>

extern std::set<std::string> supportCgiExtension;
extern std::map<std::string, std::string> supportDataType;

bool isCGI(std::string& extension);
std::string getMIMEType(std::string extension);
std::string getExtension(const std::string& type);

#endif