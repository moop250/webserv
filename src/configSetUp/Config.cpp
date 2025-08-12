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

t_ServerData    Config::getServerData(int index, std::string param) const
{
    try
    {
       return (_servers[index]);
    }
    catch(...)
    {
        Error("Overflow on servers array");
        throw OutOfBoundsExeption();
    }
}

void    Config::setServerData(t_ServerData data)
{
    (void)data;
}

void    Config::parseContent()
{
    return ;
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
    t_ServerData    print;

    std::cout << "In operator <<\n";
    print = conf.getServerData(0, "all");
    std::cout << "Wesh\n";
    stream << "Server name : " << print.server_name
            << "Root path   : " << print.root
            << "Max client buf Size : " << print.client_max_body_size << std::endl;
    for (std::vector<std::string>::iterator i = print.listeners.begin(); i != print.listeners.end(); ++i)
        stream << "Listener : " << *i << '\n';
    for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); ++i)
    {
        static int iteration = 0;
        stream << "Locations :" << iteration++  << '\n'
            << "\tLocation ID     :" << i->loc << '\n'
            << "\tRedirection     : " << i->redirection << '\n'
            << "\tRoot path       : " << i->root << '\n'
            << "\tAuto index status : " << (i->autoIndex ? "On" : "OFF") << '\n'
            << "\tDefault file    : " << i->defaultFile << '\n'
            << "\tUpload storage : " << i->uploadDir << '\n'
            << "\tCgi external entity : " << i->cgi_ext << '\n'
            << "\tCgi path to interpreter : " << i->cgi_path << '\n'
            << "\tMethods allowed : ";
        for (int j = 0; j < 5; j++)
            stream << i->methods[j];
        stream << '\n';
    }
    std::cout << "Out\n"; 
    return (stream);
}
