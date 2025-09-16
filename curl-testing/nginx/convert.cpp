#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ostream>

void	replace(std::string &s, std::string to_find, std::string to_replace)
{
	size_t	init = s.find(to_find);

	if (init == std::string::npos)
		return ;
	s.replace(init, to_find.length(), to_replace);
	std::cout << "New line\n";
}

void	convert(std::ifstream &file, bool to_webserv)
{
	const int			nbTokens = 12;
	std::string			line;
	std::stringstream	to_write;
	std::string	myTokens[] = {
		"<listen>", "<host>", "<server_name>", "<html_index>",
		"<root>", "<client_max_body_size>", "<autoindex>",
		"<error_page>", "<upload_storage>", "<CGI>", "<allow>",
		"<return>"
	};
	std::string	nginxTokens[] = {
		"listen", "host", "server_name", "index", "root",
		"client_max_body_size", "autoindex", "error_page",
		"upload", "cgi", "allow", "return"
	};

	while (std::getline(file, line))
	{
		for (int i = 0; i < nbTokens; i++)
		{
			if (to_webserv)
			{
				if (line.find(nginxTokens[i]))
					replace(line, nginxTokens[i], myTokens[i]);
			}
			else
			{
				if (line.find(myTokens[i]))
					replace(line, myTokens[i], nginxTokens[i]);
			}
		}
		to_write << line;
		to_write << '\n';
	}

	std::ofstream	newFile("converted.config");
	newFile << to_write.str().c_str();
	return ;
}

int	main(int ac, char **av)
{
	std::cout << "Converting file..." << std::endl;

	if (ac != 3)
	{
		std::cout << "Bad arguments...\n"
			<< "try : ./convert [filename.config] [0 | 1]" << std::endl;
		return (1);
	}

	std::ifstream	file(av[1]);
	
	if (!file.is_open())
	{
		std::cout << "File does not exist\n";
		return (1);
	}
	convert(file, atoi(av[2]));
	
	return (0);
}
