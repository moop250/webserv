#ifndef CONFIGERROR_HPP
# define CONFIGERROR_HPP

# include "Config.hpp"

class ConfigError : public Config
{
    private:
        std::string _errorLine;
        bool        _isConfigValid;
    public:
        ConfigError(/* args */);
        ConfigError(const ConfigError &);
        ~ConfigError();
};

ConfigError::ConfigError() : Config()
{
}

ConfigError::ConfigError(const ConfigError &ce) : Config(ce)
{
    return ;
}

ConfigError::~ConfigError()
{
}

#endif