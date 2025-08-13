#include "Config.hpp"

const t_Location    default_location_values = {
    .loc = "Undefine loc",
    .methods = {"NONE", "NONE", "NONE", "NONE", "NONE"},
    .redirection = "Undefined redirection",
    .root = "/",
    .autoIndex = false,
    .defaultFile = "Undefined default file",
    .uploadDir = "Undefined upload directory",
    .cgi_ext = "No external CGI",
    .cgi_path = "Undefined CGI path"
};

const t_ServerData  default_server_values = {
    .server_name = "No server name",
    .client_max_body_size = 1,
    .root = "/"
};

void    Config::initTokenMaps()
{
    static const char *locTokenMap[] = {
        "allow_methods",
        "return",
        "root",
        "autoindex",
        "index",
        "upload_storage",
        "cgi_ext",
        "cgi_path",
        NULL
    };
    static const char *mainTokenMap[] = {
        "listen",
        "server_name",
        "error_page",
        "client_max_body",
        "root", 
        "location",
        NULL
    };
    
    for (int i = 0; i < LOCATION_TOKEN_COUNT && locTokenMap[i]; i++)
        _locTokenMap[i] = locTokenMap[i];
    for (int i = 0; i < TOKEN_TYPE_COUNT && mainTokenMap[i]; i++)
        _mainTokenMap[i] = mainTokenMap[i];
}

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile), _nbServers(1)
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

    _dfile->append("\nServers initialise at 0");
    initTokenMaps();
    _dfile->append("\nToken Maps content initialise\n");
    _servers.push_back(default_server_values);
    _servers.at(0).locations.push_back(default_location_values);
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

t_ServerData    Config::getServerData(int serverID) const
{
    if (_servers.empty())
        return (default_server_values);
    else
    {
        std::stringstream msg;

        msg << "Server N* " << serverID << " not empty\nData : \n";
        _dfile->append(msg.str().c_str());
        return (_servers.back());
    }
    try
    {
       return (_servers.at(serverID));
    }
    catch(...)
    {
        throw MissingParamException();
    }
}

int Config::getNbServers() const
{
    return _nbServers;
}

void    Config::setServerData(t_ServerData data)
{
    (void)data;
}

void    reset(t_ServerData &serv, std::string &content, size_t &pos, size_t &rBegin, size_t &rEnd)
{
    //  revoir logique
    serv = default_server_values;
    pos = 0;
    rBegin = content.find_first_of("server");
    if (rBegin == -1)
    {
        rEnd = rBegin;
        return ;
    }
    while (rBegin != content.length() && content[rBegin] != '{')
        rBegin++;
    rEnd = content.find_last_of('}');
}

void    reset(t_Location &loc, std::string &content, size_t &pos, size_t &rBegin, size_t &rEnd)
{
    //  revoir logique
    loc = default_location_values;
    pos = 0;
    rBegin = content.find_first_of("location");
    if (rBegin == std::string::npos)
    {
        rEnd = rBegin;
        return ;
    }
    while (rBegin != content.length() && content[rBegin] != '{')
        rBegin++;
    rEnd = content.find_last_of('}');
}

size_t  Config::findToken(std::string content, size_t range[2], e_TokenType i)
{
    size_t  pos = content.find(_mainTokenMap[i]);

    std::cout << "Token looking for : " << _mainTokenMap[i]
        << "\nposition : " << pos
        << "\nrange begine at " << range[0]
        << "\nrange ends at " << range[1] << std::endl;
    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);
    return (pos);
}

size_t  Config::findToken(std::string content, size_t range[2], e_LocationToken i)
{
    size_t  pos = content.find(_locTokenMap[i]);

    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);
    return (pos);
}

void    sanitizeLine(std::string &line)
{
    (void)line;
    return ;
}

std::string getTokenLine(std::string token, size_t pos)
{
    std::string line = "default line";
    (void)token;
    (void)pos;
    return (line);
}

void    Config::assignToken(t_Location &loc, std::string content, size_t pos, int type)
{
    std::string tokenLine = "";

    tokenLine = getTokenLine(_locTokenMap[type], pos);
    sanitizeLine(tokenLine);
    switch (type)
    {
        case METHODS:
            loc.methods[0] = tokenLine.substr(0, tokenLine.find_first_of(' '));
            pos = loc.methods[0].length();
           // for (int i = 1; i < 5; i++)
           // {
           //     if (i != 1)
           //         pos += tokenLine
           //     loc.methods[i] = tokenLine.substr(find)
            break;
        case HTTP_REDIRECTION:
            break ;
        case FILE_PATH:
            break ;
        case DIR_LISTING:
            break ;
        case UPLOAD_STORAGE:
            break ;
        case CGI_EXTENSION:
            break ;
        case CGI_PATH:
            break ;
        default:
            break;
    }
}

void    Config::parseLocation(t_ServerData &serv, std::string &content)
{
    t_Location  loc = default_location_values;
    size_t      lpos = 0;
    size_t      locRange[2] = {0, 0};
    
    return ;
    reset(loc, content, lpos, locRange[BEGIN], locRange[END]);
    if (locRange[BEGIN] == std::string::npos)
    {
        _dfile->append("RANGE FAILED");
        return ;
    }
    for (int i = 0; i < LOCATION_TOKEN_COUNT; i++)
    {
        if (!(lpos = findToken(content, locRange, static_cast<e_LocationToken>(i))))
            continue ;
        assignToken(loc, content, lpos, static_cast<e_LocationToken>(i));
    }
    serv.locations.push_back(loc);
}

void    eraseLine(std::string &content, std::string line)
{
    size_t  from = content.find(line), to = 0;

    if (from == std::string::npos)
        return ;
    to = from + line.length();
    content.erase(from, to);
    std::cout << YELLOW << "line suppressed : " << line << RESET <<std::endl;
    return ;
}

void    Config::assignToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    std::string tokenLine = "default token line";
    std::string path;
    int         nb;

    tokenLine = getTokenLine(_mainTokenMap[type], pos);
    eraseLine(content, tokenLine);
    sanitizeLine(tokenLine);
    switch (type)
    {
        case LISTENER:
            serv.listeners.push_back(tokenLine);
            break ;
        case SERVER_NAME:
            serv.server_name = tokenLine;
            break ;
        case ERROR_PAGE:
            //nb = extractFirstNumber(tokenLine);
            //path = extractPath(tokenLine);
            //serv.error_pages.insert(std::make_pair(nb, path));
            break ;
        case CLIENT_MAX_BODY_SIZE:
            serv.client_max_body_size = atoi(tokenLine.c_str());
            break ;
        case ROOT_PATH:
            serv.root = tokenLine;
            break ;
        case LOCATION:
            serv.locations.push_back(default_location_values);
            break ;
            parseLocation(serv, content);
            break ;
        default:
            break ;
    }
}

void    Config::parseContent()
{
    std::string     trim = _content;
    t_ServerData    serv;
    size_t          servPos;
    size_t          servRange[2] = {0, 0};

    std::stringstream   dbug;

    int a = 0;
    while (!trim.empty())
    {
        for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        {
            reset(serv, trim, servPos, servRange[0], servRange[1]);
            if (servRange[0] == std::string::npos)
                break ;
            dbug << "Server range begin at " << servRange[BEGIN]
                << " and ends at " << servRange[END] << '\n';
            _dfile->append(dbug.str().c_str());
            if (!(servPos = findToken(trim, servRange, static_cast<e_TokenType>(i))))
                continue ;
            _dfile->append("TOKEN FOUND");
            assignToken(serv, trim, servPos, i);
        }
        _dfile->append("Pushing data to server's config member");
        _servers.push_back(serv);
        _dfile->append("Push success");
        trim.erase(trim.find("server"), 0);
        break ;
    }
    std::cout << "//////////////////////\n" << _content << std::endl;
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

    for (int i = 0; i < conf.getNbServers(); i++)
    {
        print = conf.getServerData(0);
        stream << "\tServer name : " << print.server_name << '\n'
                << "\tRoot path   : " << print.root << '\n'
                << "\tMax client buf Size : " << print.client_max_body_size << std::endl;
        for (std::vector<std::string>::iterator i = print.listeners.begin(); i != print.listeners.end(); ++i)
            stream << "\tListener : " << *i << '\n';
        for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); ++i)
        {
            static int iteration = 0;
            stream << "\tLocations :" << iteration++  << '\n'
                << "\t\tLocation ID     :" << i->loc << '\n'
                << "\t\tRedirection     : " << i->redirection << '\n'
                << "\t\tRoot path       : " << i->root << '\n'
                << "\t\tAuto index status : " << (i->autoIndex ? "On" : "OFF") << '\n'
                << "\t\tDefault file    : " << i->defaultFile << '\n'
                << "\t\tUpload storage : " << i->uploadDir << '\n'
                << "\t\tCgi external entity : " << i->cgi_ext << '\n'
                << "\t\tCgi path to interpreter : " << i->cgi_path << '\n'
                << "\t\tMethods allowed : ";
            for (int j = 0; j < 5; j++)
                stream << i->methods[j];
            stream << '\n';
        }
    }
    return (stream);
}
