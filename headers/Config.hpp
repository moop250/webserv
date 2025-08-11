#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"

class Config
{
    std::string _serversNames;
    public:
        Config();
        Config(const Config &);
        ~Config();

        Config  &operator=(const Config &);
};

Config::Config() :
    _serversNames("Uname server")
{
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


#endif