#ifndef ERROR_HPP
# define ERROR_HPP

# include "StdLibs.hpp"

class Error
{
	public:
		Error(const char* error, const char* function, const char *file, int line);
};

Error::Error(const char *error, const char *function, const char *file, int line)
{
	std::cout << RED << "Error :\t" << WHITE << '[' << YELLOW << error << WHITE << "]"
		<< "\nAt line     : " << YELLOW << line
		<< WHITE << "\nIn file     : " << YELLOW << file
		<< WHITE << "\nIn function : " << YELLOW << function
		<< RESET << std::endl;
}

#endif
