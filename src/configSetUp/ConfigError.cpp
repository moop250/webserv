#include "ConfigError.hpp"

ConfigError::ConfigError() :
    Config(), _suggsestedToken(""), _block(""),
      _errorLine(""), _lineCount(0), _isValid(1),
      _line(LINE_EMPTY)
{
}

ConfigError::ConfigError(const Config &c) :
    Config(c), _suggsestedToken(""), _block(""),
      _errorLine(""), _lineCount(0), _isValid(1),
      _line(LINE_EMPTY)
{
    static bool (ConfigError::*CheckersArr[CONFIG_CHECKERS])() = {
        &ConfigError::checkBrackets,
        &ConfigError::checkNbServers,
        &ConfigError::checkTokens,
        &ConfigError::checkLinesFormat
    };
    static const char *msg[CONFIG_CHECKERS] = {
        "Unmatching bracket. Missing '{' or '}'", "No server specified in config file",
        "Line does not match format", "Missing token"
    };

    for (int i = 0; i < CONFIG_CHECKERS; i++)
        checkers[i] = CheckersArr[i];
    for (int i = 0; i < CONFIG_CHECKERS + LINE_CHECKERS; i++)
        errors[i] = msg[i];
    _isValid = checkConfig();
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

