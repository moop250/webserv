#include <iostream>
#include <string>
//#include <ifstream>
//#include <ofstream>
#include <sstream>
#include <ostream>

void	convert(std::ifstream &file, bool to_webserv)
{
	const int	nbTokens = 12;
	std::string	line;
	std::sstream	to_write;
	std::string	mytokens[] = {
		"<port>", "<host>", "<server_name>", "<html_index>",
		"<root>", "<client_max_body_size>", "<autoindex>",
		"<error_page>", "<upload_storage>", "<CGI>", "<allow">,
		"<return>"
	};
	std::string	nginxTokens[] = {
		"port", "host", "server_name", "index", "root",
		"client_max_body_size", "autoindex", "error_page",
		"upload", "cgi", "allow", "return"
	};

	while ((std::getline(file, line)))
	{
		for (int i = 0; i < nbTokens; i++)
		{
			if (to_webserv)
			{
				if (line.find(nginxTokens[i]))
					line.replace(nginxTokens[i], myTokens[i]);
			}
			else
			{
				if (line.find(tokens[i]))
					line.replace(myTokens[i], nginxTokens[i]);
			}
		to_write << line;
		to_write << '\n';
	}
	file.name().erase(".config");
	std::ofstream	newFile(file.name() + "converted.config");
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

	convert(file);
	return (0);
}
