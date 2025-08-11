#include "Config.hpp"

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
