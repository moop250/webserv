#include "Config.hpp"

const t_ServerData  default_server_values = {
    .server_name = "No server name",
    .root = "/",
    .index = "No file.html undefined",
    .upload_storage = "Upload dir undefined",
    .cgi_ext = "cgi extention undefined",
    .cgi_path = "cgi path undefined",
    .client_max_body_size = 1,
    .autoindex = false
};

const t_Location    default_location_values = {
    .active = false,
    .data = default_server_values
};

//	server_name 127.0.0.1
//	listen 5050
//	root ./public/website1
//	index index.html
//	max_client_body_size 2
//    allow  GET DELETE
//    autoindex on
//	error_page 404 ./public/error_pages/404.html
//    upload ./public/website2/
//	location /home {

void    Config::initTokenMaps()
{
    static const char *mainTokenMap[] = {
        "listen",
        "server_name",
        "root",
        "index",
        "autoindex",
        "error_page",
        "upload_storage",
        "cgi_ext",
        "cgi_path",
        "client_max_body",
        "allowed_methods",
        "location",
        NULL
    };
    
    for (int i = 0; i < TOKEN_TYPE_COUNT && mainTokenMap[i]; i++)
        _Tokens[i] = mainTokenMap[i];
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
    _servers.at(0).hosts.push_back("Undefined Host");
    _servers.at(0).listeners.push_back("Undefinded listeners");
    _servers.at(0).methods.push_back("GET");
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
    rBegin = content.find("location", pos);
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
    size_t  pos = content.find(_Tokens[i], range[BEGIN]);

    std::cout << "Token looking for : " << _Tokens[i]
        << "\nposition : " << pos
        << "\nrange begine at " << range[0]
        << "\nrange ends at " << range[1] << std::endl;
    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);
    return (pos);
}

void    sanitizeLine(std::string &line)
{
    (void)line;
    return ;
}

std::string getTokenLine(std::string content, std::string token, size_t pos)
{
    size_t      len = 0;//content.find_first_of('\n', pos);
    for (; content.c_str()[pos + len] && content.c_str()[pos + len] != '\n'; len++)
        len++;
    std::string line = content.substr(pos, len);

    std::cout << CYAN << "Line found : " << line << RESET << std::endl;
    std::cout << "Line length : " << len << std::endl;
    return (line);
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

//void    Config::assignToken(t_Location &loc, std::string content, size_t pos, int type)
//{
//    std::string tokenLine = "";
//
//    tokenLine = getTokenLine(content, _locTokenMap[type], pos);
//    eraseLine(content, tokenLine);
//    sanitizeLine(tokenLine);
//    switch (type)
//    {
//        case METHODS:
//            loc.methods[0] = tokenLine.substr(0, tokenLine.find_first_of(' '));
//            pos = loc.methods[0].length();
//           // for (int i = 1; i < 5; i++)
//           // {
//           //     if (i != 1)
//           //         pos += tokenLine
//           //     loc.methods[i] = tokenLine.substr(find)
//            break;
//        case HTTP_REDIRECTION:
//            break ;
//        case FILE_PATH:
//            break ;
//        case DIR_LISTING:
//            break ;
//        case UPLOAD_STORAGE:
//            break ;
//        case CGI_EXTENSION:
//            break ;
//        case CGI_PATH:
//            break ;
//        default:
//            break;
//    }
//}

void    Config::parseLocation(t_ServerData &serv, std::string &content)
{
    serv.locations.push_back(default_location_values);
}

void    Config::assignToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    std::string tokenLine = "default token line";
    std::string path;
    int         nb;

    tokenLine = getTokenLine(content, _Tokens[type], pos);
    eraseLine(content, tokenLine);
    sanitizeLine(tokenLine);
    switch (type)
    {
        case LISTEN:
            serv.listeners.push_back(tokenLine);
            break ;
        case SERVER_NAME:
            serv.server_name = tokenLine;
            break ;
        case ROOT_PATH:
            serv.root = tokenLine;
            break ;
        case HTLM_INDEX:
            break ;
        case AUTOINDEX:
            break ;
        case ERROR_PAGE:
            break ;
        case UPLOAD_STORAGE:
            break ;
        case CGI_EXTENTION:
            break ;
        case CGI_PATH:
            break ;
        case CLIENT_MAX_BODY_SIZE:
            serv.client_max_body_size = atoi(tokenLine.c_str());
            break ;
        case LOCATION:
            serv.locations.push_back(default_location_values);
            break ;
        //    parseLocation(serv, content);
          //  break ;
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

    if (trim.empty())
        return _servers.push_back(default_server_values);
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
        _nbServers++;
        break ;
    }
    std::cout << "NB of servers = " << _nbServers << std::endl;
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

    std::cout << ROSE << "PRINTING OPERATOR\n";
    for (int i = 0; i < conf.getNbServers(); i++)
    {
        print = conf.getServerData(1);
        stream  << "\tServer name    : " << print.server_name << '\n'
                << "\tRoot path      : " << print.root << '\n'
                << "\tIndex file     : " << print.index << '\n'
                << "\tAuto index     : " << (print.autoindex ? "On" : "OFF") << '\n'
                << "\tUpload storage : " << print.upload_storage << '\n'
                << "\tCgi external   : " << print.cgi_ext << '\n'
                << "\tCgi path       : " << print.cgi_path << '\n'
                << "\tMax client siz : " << print.client_max_body_size << std::endl;
        std::cout << "OK\n";
        stream << "\tMethods : ";
        for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); ++i)
            stream << " " << *i;
        stream << '\n';
        stream << "\tHosts : ";
        for (std::vector<std::string>::iterator i = print.hosts.begin(); i != print.hosts.end(); ++i)
            stream << " " << *i;
        stream << '\n' << "\tListeners : ";
        for (std::vector<std::string>::iterator i = print.listeners.begin(); i != print.listeners.end(); ++i)
            stream << " " << *i;
        stream << '\n';
        for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); ++i)
        {
            static int iteration = 0;
            stream << "\tLocations :" << iteration++  << '\n'
                << "\t\tLocation ID             :" << std::to_string(i->data.locations.size()) << '\n'
                << "\t\tRoot path               : " << i->data.root << '\n'
                << "\t\tAuto index status       : " << (i->data.autoindex ? "On" : "OFF") << '\n'
                << "\t\tHTLM index file         : " << i->data.index << '\n'
                << "\t\tUpload storage          : " << i->data.upload_storage << '\n'
                << "\t\tCgi external entity     : " << i->data.cgi_ext << '\n'
                << "\t\tCgi path to interpreter : " << i->data.cgi_path << '\n'
                << "\t\tMethods allowed : ";
            for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); ++i)
                stream << *i << '\n';
            stream << '\n';
        }
    }
    return (stream);
}
