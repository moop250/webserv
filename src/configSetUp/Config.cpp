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

void    Config::printServers() const
{
    return ;
}

void    Config::initTokenMaps()
{
    static const char *mainTokenMap[] = {
        "host",
        "listen",
        "server_name",
        "root",
        "index",
        "autoindex",
        "error_page",
        "upload_storage",
        "cgi_ext",
        "cgi_path",
        "client_max_body_size",
        "allow_methods",
        "location",
        NULL
    };
    
    for (int i = 0; i < TOKEN_TYPE_COUNT && mainTokenMap[i]; i++)
        _Tokens[i] = mainTokenMap[i];
}

t_ServerData    getDefaultServ(bool with_location)
{
    t_ServerData    defaultServ = default_server_values;

    defaultServ.locations.push_back(default_location_values);
    if (!with_location)
        return defaultServ;
    defaultServ.hosts.push_back("Undefined Host");
    defaultServ.listeners.push_back("Undefinded listeners");
    defaultServ.methods.push_back("GET");
    return (defaultServ);
}

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile), _nbServers(0)
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
    _servers.push_back(getDefaultServ(0));
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
        return (_servers.at(serverID));
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
    pos = 0;
    rBegin = content.find("server");
    if (rBegin == std::string::npos)
    {
        rEnd = rBegin;
        return ;
    }
    while (rBegin != content.length() && content[rBegin] != '{')
        rBegin++;
    rEnd = content.find('{', rBegin + 1);
}

void    reset(t_Location &loc, std::string &content, size_t &pos, size_t &rBegin, size_t &rEnd)
{
    //  revoir logique
    pos = 0;
    rBegin = content.find("location", pos);
    if (rBegin == std::string::npos)
    {
        rEnd = rBegin;
        return ;
    }
    while (rBegin != content.length() && content[rBegin] != '{')
        rBegin++;
    rEnd = content.find('{', rBegin + 1);
}

size_t  Config::findToken(std::string content, size_t range[2], e_TokenType i)
{
    size_t  pos = content.find(_Tokens[i], range[BEGIN]);

    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);

    while (pos != content.length() - 5 && pos != std::string::npos && (!isspace(content[pos - 1]) || !isspace(content[pos + _Tokens[i].length()])))
    {
//        std::cout << "It was " << content[pos - 1] << " and " << content[pos + _Tokens[i].length()] << std::endl;
        pos += 1;
        pos = content.find(_Tokens[i], pos);
    }
//    std::cout << "Token looking for : " << _Tokens[i]
//        << "\nposition : " << pos
//        << "\nrange begine at " << range[0]
//        << "\nrange ends at " << range[1] << std::endl;
    if (pos == std::string::npos)
        return (0);
    return (pos);
}

void    sanitizeLine(std::string &line)
{
    int to_remove = 0;

    while (!isspace(line[to_remove]))
        to_remove++;
    line.erase(0, to_remove + 1);
    return ;
}

std::string getTokenLine(const std::string &content, const std::string &token, size_t pos)
{
    size_t end = content.find(';', pos);

    if (end == std::string::npos)
        end = content.size();
    std::string line = content.substr(pos, end - pos);
//    std::cout << CYAN << "Line found : " << line << RESET << std::endl;
//    std::cout << "Line length : " << (end - pos) << std::endl;
    return line;
}

void eraseLine(std::string &content, const std::string &line)
{
    size_t  from = content.find(line);

    if (from == std::string::npos)
        return;    
    content.erase(from, line.length());
    if (from < content.size() && content[from] == ';')
        content.erase(from, 1);
}

//void    Config::assignToken(t_Location &loc, std::string content, size_t pos, int type)
//{
//}

void    Config::parseLocation(t_ServerData &serv, std::string &content)
{
    serv.locations.push_back(default_location_values);
}

void    Config::assignToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    std::string tokenLine = "default token line";
    std::string path = "no path";
    int         nb = 404;

    tokenLine = getTokenLine(content, _Tokens[type], pos);
    eraseLine(content, tokenLine);
    sanitizeLine(tokenLine);
    std::cout << ROSE << "line : " << tokenLine << std::endl << RESET;
    std::cout << "type : " << type << std::endl;
    switch (type)
    {
        case HOST:
            serv.hosts.push_back(tokenLine);
            break ;
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
            serv.index = tokenLine;
            break ;
        case AUTOINDEX:
            (tokenLine == "ON" ? serv.autoindex = true : serv.autoindex = false);
            break ;
        case ERROR_PAGE:
            serv.error_pages.insert(std::make_pair(nb, path));
            break ;
        case UPLOAD_STORAGE:
            serv.upload_storage = tokenLine;
            break ;
        case CGI_EXTENTION:
            serv.cgi_ext = tokenLine;
            break ;
        case CGI_PATH:
            serv.cgi_path = tokenLine;
            break ;
        case CLIENT_MAX_BODY_SIZE:
            serv.client_max_body_size = atoi(tokenLine.c_str());
            break ;
        case METHODS:
            serv.methods.push_back(tokenLine);
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

void    assignDefaultToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    (void)content;
    (void)pos;
     switch (type)
    {
        case HOST:
            serv.hosts.push_back("UNDEFINED");
                break ;
        case LISTEN:
            serv.listeners.push_back("UNDEFINED");
            break ;
        case SERVER_NAME:
            serv.server_name = "UNDEFINED";
            break ;
        case ROOT_PATH:
            serv.root = "UNDEFINED";
            break ;
        case HTLM_INDEX:
            serv.index = "UNDEFINED";
            break ;
        case AUTOINDEX:
            serv.autoindex = false;
            break ;
        case ERROR_PAGE:
            serv.error_pages.insert(std::make_pair(404, "UNDEFINED"));
            break ;
        case UPLOAD_STORAGE:
            serv.upload_storage = "UNDEFINED";
            break ;
        case CGI_EXTENTION:
            serv.cgi_ext = "UNDEFINED";
            break ;
        case CGI_PATH:
            serv.cgi_path = "UNDEFINED";
            break ;
        case CLIENT_MAX_BODY_SIZE:
            serv.client_max_body_size = 0;
            break ;
        case METHODS:
            serv.methods.push_back("NO METHOD");
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
    _servers.pop_back();    // rm default
    while (!trim.empty())
    {
        serv = getDefaultServ(0);
        for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        {
            reset(serv, trim, servPos, servRange[0], servRange[1]);
            if (servRange[0] == std::string::npos)
                break ;
            dbug << "Server range begin at " << servRange[BEGIN]
                << " and ends at " << servRange[END] << '\n';
            dbug << "Token treated is " << _Tokens[i] << '\n';
            _dfile->append(dbug.str().c_str());
            dbug.str("");
            dbug.clear();
            if (!(servPos = findToken(trim, servRange, static_cast<e_TokenType>(i))))
                assignDefaultToken(serv, trim, servPos, i);
            else
                assignToken(serv, trim, servPos, i);
        }
        _dfile->append("Pushing data to server's config member");
        _servers.push_back(serv);
        _dfile->append("Push success");
        trim.erase(trim.find("server"), 8);
        _nbServers++;
        break ;
    }
    std::cout << trim << std::endl;
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
        print = conf.getServerData(i);
        stream // << "\tHost name      : " << print.hosts.at(0) << '\n'
                << "\tServer name    : " << print.server_name << '\n'
                << "\tRoot path      : " << print.root << '\n'
                << "\tIndex file     : " << print.index << '\n'
                << "\tAuto index     : " << (print.autoindex ? "On" : "OFF") << '\n'
                << "\tUpload storage : " << print.upload_storage << '\n'
                << "\tCgi external   : " << print.cgi_ext << '\n'
                << "\tCgi path       : " << print.cgi_path << '\n'
                << "\tMax client siz : " << print.client_max_body_size << std::endl;
        std::cout << "OK\n";
        stream << "\tMethods : ";
        for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); i++)
            stream << " " << *i;
        stream << '\n';
        stream << "\tHosts : ";
        for (std::vector<std::string>::iterator i = print.hosts.begin(); i != print.hosts.end(); i++)
            stream << " " << *i;
        stream << '\n' << "\tListeners : ";
        for (std::vector<std::string>::iterator i = print.listeners.begin(); i != print.listeners.end(); i++)
            stream << " " << *i;
        stream << "\n";
        for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); i++)
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
            stream << "\n\n";
        }
        stream << "\n\n";
    }
    return (stream);
}
