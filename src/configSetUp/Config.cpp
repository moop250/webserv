#include "Config.hpp"

Config::Config(std::string fileName, Debug &dfile) :
    _servers(NULL) ,_dfile(&dfile)
{
    std::ifstream   readFile(fileName.c_str());
    char            buf[10000];
    static const std::map<std::string, e_TokenType> mainTokenMap = {
        {"listen", LISTENER},
        {"server_name", SERVER_NAME},
        {"error_page", ERROR_PAGE},
        {"client_max_body_size", CLIENT_MAX_BODY_SIZE},
        {"root", ROOT_PATH},
        {"location", LOCATION}
    };

    static const std::map<std::string, e_LocationToken> locTokenMap = {
        {"methods", METHODS},
        {"return", HTTP_REDIRECTION},
        {"root", FILE_PATH},
        {"autoindex", DIR_LISTING},
        {"index", DEFAULT_FILE},
        {"upload_store", UPLOAD_STORAGE},
        {"cgi_ext", CGI_EXTENSION},
        {"cgi_path", CGI_PATH}
    };

    //  read config file
    for (int i = 0; i < 10000; i++)
        buf[i] = 0;
    readFile.read(buf, sizeof(readFile));
    _content = buf;
	_dfile->append("content of config file read\nContent :\n");
    _dfile->append(buf);

    //  Main tokens to look
    for (int i = 0; i < NUM_MAIN_TOKENS; i++)
        _MainTokens[i] = mtokens[i];
    
    //  Locations tokens to look
    for (int i = 0; i < NUM_LOC_TOKENS; i++)
        _LocTokens[i] = ltokens[i];

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
 //   int i = 0;
//
 //   if (_servers)
 //       _servers->assign(...)
 //   return ;
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
