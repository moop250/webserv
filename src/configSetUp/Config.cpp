#include "Config.hpp"

void    Config::initTokenMaps()
{
    _locTokenMap.insert(std::make_pair("methods", METHODS));
    _locTokenMap.insert(std::make_pair("return", HTTP_REDIRECTION));
    _locTokenMap.insert(std::make_pair("root", FILE_PATH));
    _locTokenMap.insert(std::make_pair("autoindex", DIR_LISTING));
    _locTokenMap.insert(std::make_pair("index", DEFAULT_FILE));
    _locTokenMap.insert(std::make_pair("upload_store", UPLOAD_STORAGE));
    _locTokenMap.insert(std::make_pair("cgi_ext", CGI_EXTENSION));
    _locTokenMap.insert(std::make_pair("cgi_path", CGI_PATH));

    _mainTokenMap.insert(std::make_pair("listen", LISTENER));
    _mainTokenMap.insert(std::make_pair("server_name", SERVER_NAME));
    _mainTokenMap.insert(std::make_pair("error_page", ERROR_PAGE));
    _mainTokenMap.insert(std::make_pair("client_max_body_size", CLIENT_MAX_BODY_SIZE));
    _mainTokenMap.insert(std::make_pair("root", ROOT_PATH));
    _mainTokenMap.insert(std::make_pair("location", LOCATION));
}

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile), _servers(NULL)
{
    std::ifstream   readFile(fileName.c_str());
    char            buf[10000];

    //  read config file
    for (int i = 0; i < 10000; i++)
        buf[i] = 0;
    // check buffersize ?
    readFile.read(buf, sizeof(readFile));
    _content = buf;
	_dfile->append("\ncontent of config file read\nContent :\n");
    _dfile->append(buf);

    initTokenMaps();
    _dfile->append("\nToken Maps content initialise\n");
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

bool    Config::checkServerData(int index) const
{
    (void)index;
    return (false);
}

t_ServerData    Config::getServerData(int index) const
{
    try
    {
        return (_servers[index]);
    }
    catch(const std::exception& e)
    {
        throw OutOfBoundsExeption();
    }
}

void    Config::setServerData(t_ServerData data)
{
    (void)data;
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

const char  *Config::OutOfBoundsExeption::what() const throw()
{
    return "Data to be reach is out of bounds";
}

std::ostream    &operator<<(std::ostream &stream, Config &conf)
{
    (void)stream;
    (void)conf;
    return (stream);
}
