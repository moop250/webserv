#include "Error.hpp"

Error::Error(const char *error, const char *function, const char *file, int line)
{
	std::cout << RED << "Error :\t" << WHITE << '[' << YELLOW << error << WHITE << "]";
	if (file)
		std::cout << WHITE << "\nIn          : " << YELLOW << file;
	if (line)
		std::cout << WHITE << "\nAt line     : " << YELLOW << line;
	if (function)
		std::cout << WHITE << "\nIn block    : " << YELLOW << function;
	std::cout << RESET << std::endl;
}

ErrorDebug::ErrorDebug(const char *error)
{
	std::cerr << error << std::endl;
}

ErrorDebug::ErrorDebug(Debug &stream, const char *error)
{
	if (!stream.file.is_open())
		return ;
	stream.file << "Error :\t" << '[' << error << "]";
	stream.file << std::endl;
}

ErrorDebug::ErrorDebug(Debug &stream, const char *error, const char *function, const char *file, int line)
{
	if (!stream.file.is_open())
		return ;
	stream.file << "Error :\t" << '[' << error << "]";
	if (file)
		stream.file << "\nIn          : " << file;
	if (line)
		stream.file << "\nAt line     : " << line;
	if (function)
		stream.file << "\nIn function : " << function;
	stream.file << std::endl;
}
