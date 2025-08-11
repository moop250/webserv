#include "Config.hpp"

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile)
{
    std::ifstream   readFile(fileName.c_str());
    char            buf[10000];

    //  read config file
    for (int i = 0; i < 10000; i++)
        buf[i] = 0;
    readFile.read(buf, sizeof(readFile));
    _content = buf;
	_dfile->append("content of config file read");
    _dfile->append(buf);
    //  set tokens to look
    //      listener
    //      host
    //      root path
    //      client buffer size
    //      error Pages
    //       
    //      Location 1
    //          methods
    //          extra mode
    //
    //      Location ...
    
    //      Location n
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