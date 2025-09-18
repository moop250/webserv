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

std::string	convert(std::ifstream &file, bool to_webserv)
{
	const int			nbTokens = 12;
	std::string			line;
	std::stringstream	to_write;
	std::string	myTokens[] = {
		"<listen>", "<host>", "<server_name>",
		"<root>", "<client_max_body_size>", "<autoindex>",
		"<error_page>", "<upload_storage>", "<CGI>", "<allow>",
		"<return>", "<html_index>"
	};
	std::string	nginxTokens[] = {
		"listen ", "host ", "server_name ", "root ",
		"client_max_body_size ", "autoindex ", "error_page ",
		"upload ", "cgi ", "allow ", "return ", "index "
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

	return to_write.str();
}

int	main(int ac, char **av)
{
	std::string	name = av[1];
	std::string	webserv_convert = av[2];

	std::cout << "Converting file..." << std::endl;

	if (ac != 3)
	{
		std::cout << "Bad arguments...\n"
			<< "try : ./convert [filename.config] [0 | 1]" << std::endl;
		std::cout << "0 to nginx, 1 to webserv\n";
		return (1);
	}

	std::ifstream	file(av[1]);
	
	if (!file.is_open())
	{
		std::cout << "File does not exist\n";
		return (1);
	}
	if (name.find(".config") == std::string::npos)
	{
		std::cout << "File given doesn't respect '.config' format\n";
		return (1);
	}
	std::cout << "Converting file ";
	name.erase(name.find(".config"), 7);
	
	if (webserv_convert == "0") {
		std::cout << "to nginx format...\n"; name.append("_nginx.config");
	}
	else if (webserv_convert == "1") {
		std::cout << "to webserv format...\n"; name.append("_webserv.config");
	}
	else
		std::cout << "Where is the option?\n";

	std::ofstream	newFile(name.c_str());

	newFile << convert(file, atoi(av[2]));
	return (0);
}
