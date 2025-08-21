#include "ConfigError.hpp"

ConfigError::ConfigError() : Config(), _isValid(1), _errorLine("")
{
}

# define KO 0
# define OK 1

bool checkBracket(std::string content)
{
    int open = 0, close = 0;
    size_t  len = content.length();

    for (size_t i = 0; i < len; i++)
    {
        if (content[i] == '{')
            open++;
        if (content[i] == '}')
            close++;
    }
    if (open != close)
        return KO;
    return OK;
}

ConfigError::ConfigError(const Config &c) : Config(c), _isValid(1), _errorLine("")
{
    _isValid = 1;
    return ;
    _isValid = checkContent();
    return ;
}

ConfigError::ConfigError(const ConfigError &ce) : Config(ce), _isValid(1), _errorLine("")
{
    return ;
}

ConfigError::~ConfigError()
{
}

ConfigError &ConfigError::operator=(const ConfigError &ce)
{
    return *this;
}

bool    ConfigError::isConfigValid() const
{
    return _isValid;
}

std::string ConfigError::getErrorLine() const
{
    return _errorLine;
}

const char  *ConfigError::BadFileException::what() const throw()
{
    return "File given is empty or does not exist";
}

const char  *ConfigError::MissingParamException::what() const throw()
{
    return "Missing parameter in configuration file";
}

const char  *ConfigError::BadParamException::what() const throw()
{
    return "Parameter in configuration file is weird and was in consequence not handled";
}

const char  *ConfigError::ParseErrorExemption::what() const throw()
{
    return "I did some shit somewhere";
}

const char  *ConfigError::OutOfBoundsExeption::what() const throw()
{
    return "Data to be reach is out of bounds";
}

