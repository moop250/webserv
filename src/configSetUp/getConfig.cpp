#include "Config.hpp"
#ifndef ERROR_HPP
#include "Error.hpp"
#endif
#ifndef DEBUG_HPP
#include "Debug.hpp"
#endif

Config::Config(std::string fileName)
{
    if (fileName == "NOFILE")
    {
        throw Error("No File Name");
    }
    return ;
}

Config::Config(const Config &conf)
{
    (void)conf;
    return ;
}

Config::~Config()
{
    return ;
}

Config  &Config::operator=(const Config &)
{
    return *this;
}

const char  *Config::BadFileException::what() const throw()
{
    return "File given is empty or does not exist";
}

const char  *Config::MissingParamException::what() const throw()
{
    return "Missing parameter in configuration file";
}

const char  *Config::BadParamException::what() const throw()
{
    return "Parameter in configuration file is weird and was in consequence not handled";
}

const char  *Config::ParseErrorExemption::what() const throw()
{
    return "I did some shit somewhere";
}