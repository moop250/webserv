/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlibine <hlibine@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:30:52 by hlibine           #+#    #+#             */
/*   Updated: 2025/06/10 18:33:23 by hlibine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/core.hpp"
#include <fstream>
#include <stdexcept>

std::map<std::string, t_coreSettings> Core::parseCore(char *file) {

	std::ifstream f(file);
	if (!f.is_open())
		throw std::runtime_error("Could not open config file");

	std::map<std::string, t_coreSettings>	coreSett;


	f.close();

	return coreSett;
}