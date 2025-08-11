#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"

class Config
{
    int         _fd;
    std::string _file;
    std::string _serversNames;
    public:
        Config();
        Config(const Config &);
        ~Config();

        Config  &operator=(const Config &);
};

Config::Config() :
    _fd(-1), _file("No file"), _serversNames("Uname server")
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