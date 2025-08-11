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

#endif