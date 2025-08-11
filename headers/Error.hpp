#ifndef ERROR_HPP
# define ERROR_HPP

# include "StdLibs.hpp"
# include "Debug.hpp"

class Error
{
	public:
		Error(const char* error = "Undefined error", const char* function = NULL, const char *file = NULL, int line = 0);
};

class ErrorDebug
{
	public:
		ErrorDebug(const char *error = "Undefined error");
		ErrorDebug(Debug &stream, const char *error);
		ErrorDebug(Debug &stream, const char *error, const char* function, const char *file = NULL, int line = 0);
};

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
	stream.file << RED << "Error :\t" << WHITE << '[' << YELLOW << error << WHITE << "]";
	stream.file << RESET << std::endl;
}

ErrorDebug::ErrorDebug(Debug &stream, const char *error, const char *function, const char *file, int line)
{
	if (!stream.file.is_open())
		return ;
	stream.file << RED << "Error :\t" << WHITE << '[' << YELLOW << error << WHITE << "]";
	if (line)
		stream.file << "\nAt line     : " << YELLOW << line;
	if (file)
		stream.file << WHITE << "\nIn file     : " << YELLOW << file;
	if (function)
		stream.file << WHITE << "\nIn function : " << YELLOW << function;
	stream.file << RESET << std::endl;
}

#endif
