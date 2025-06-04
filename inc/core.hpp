/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlibine <hlibine@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:32:40 by hlibine           #+#    #+#             */
/*   Updated: 2025/06/04 19:51:14 by hlibine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

#include <map>
#include <string>
#include <vector>

typedef struct s_coreSettings {
	std::string serverName;

}	t_coreSettings;

class Core {

	private:
		int		configFd;
		void	parseCore(int fd);
		std::map<std::string, t_coreSettings>	&serverConfigs;

	public:
		Core(int fd);
		~Core();

};

#endif