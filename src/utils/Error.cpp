#include "Error.hpp"

Error::Error(const char *error, const char *function, const char *file, int line)
{
	std::cout << RED << "Error :\t" << WHITE << '[' << YELLOW << error << WHITE << "]";
	if (line)
		std::cout << "\nAt line     : " << YELLOW << line;
	if (file)
		std::cout << WHITE << "\nIn file     : " << YELLOW << file;
	if (function)
		std::cout << WHITE << "\nIn function : " << YELLOW << function;
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
	if (line)
		stream.file << "\nAt line     : " << line;
	if (file)
		stream.file << "\nIn file     : " << file;
	if (function)
		stream.file << "\nIn function : " << function;
	stream.file << std::endl;
}
