#include "nginxManip.hpp"

smanip::smanip() {}

smanip::~smanip() {}

std::string
smanip::content(
	std::string file
)
{
	std::string			line;
	std::ifstream		fread(file);
	std::string			content = "";

	if (!fread.is_open())
	{
		std::cout << "[Error] : File not found." << std::endl;
		return (std::string(""));
	}
	while (std::getline(fread, line))
	{
		content.append(line);
		content.append("\n");
	}
	return content;
}

void
smanip::insert(
	std::string in_file,
	size_t at, std::string what
)
{
	std::string	buffer = content(in_file);

	if (at >= buffer.size())
	{
		std::cout << "[Error] : Bad index" << std::endl;
		return ;
	}

	buffer.insert(at, what);

	std::ofstream	write_in(in_file);
	
	write_in << buffer;
	
	write_in.close();
}

void
smanip::remove(
	std::string in_file,
	size_t at, std::string what
)
{
	std::string	buffer = content(in_file);

	if (at >= buffer.size())
	{
		std::cout << "[Error] : Bad index" << std::endl;
		return ;
	}

	buffer.insert(at, what);

	std::ofstream	write_in(in_file);
	
	write_in << buffer;
	
	write_in.close();
}
