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
        "<host>",
        "<listen>",
        "<server_name>",
        "<root>",
        "<html_index>",
        "<autoindex>",
        "<error_page>",
        "<upload_storage>",
        "<cgi_ext>",
        "<cgi_path>",
        "<client_max_body_size>",
        "<allow>",
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
    return (defaultServ);
}

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile), _nbServers(0)
{
    std::ifstream   readFile(fileName.c_str());
    char            buf[100000];

    for (int i = 0; i < 100000; i++)
        buf[i] = 0;
    readFile.read(buf, 100000);
    _content = buf;
    _dfile->append(buf);
    initTokenMaps();
    _servers.push_back(getDefaultServ(0));
    if (_content.empty())
        throw Config::BadFileException();
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
        return (_servers.at(serverID));
}

int Config::getNbServers() const
{
    return _nbServers;
}

void    Config::setServerData(t_ServerData data)
{
    (void)data;
}

size_t  Config::findToken(std::string content, size_t range[2], e_TokenType i)
{
    size_t  pos = content.find(_Tokens[i], range[BEGIN]);

    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);
    if (pos == std::string::npos)
        return (0);
    return (pos);
}

void    Config::assignToken(t_Location &loc, std::string &content, size_t pos, int type)
{
    std::string tokenLine = "default token line";
    std::string str = "UNDEFINED";
    int         nb = -1;

    tokenLine = getTokenLine(content, _Tokens[type], pos);
    eraseLine(content, tokenLine);
    sanitizeLine(tokenLine);
    switch (type)
    {
        case ROOT_PATH:
            loc.data.root = tokenLine;
            break ;
        case HTLM_INDEX:
            loc.data.index = tokenLine;
            break ;
        case AUTOINDEX:
            loc.data.autoindex = (tokenLine == "ON" || tokenLine == "on");
            break ;
        case ERROR_PAGE:
            nb = getNb(tokenLine, _Tokens[type]);
            str = getStr(tokenLine, _Tokens[type]);
            loc.data.error_pages.insert(std::make_pair(nb, str));
            break ;
        case UPLOAD_STORAGE:
            loc.data.upload_storage = tokenLine;
            break ;
        case CGI_EXTENTION:
            loc.data.cgi_ext = tokenLine;
            break ;
        case CGI_PATH:
            loc.data.cgi_path = tokenLine;
            break ;
        case CLIENT_MAX_BODY_SIZE:
            loc.data.client_max_body_size = atoll(tokenLine.c_str());
            break ;
        case METHODS:
            for (int i = 0; !tokenLine.empty(); i++)
            {
                str = getStr(tokenLine, _Tokens[METHODS]);
                eraseLine(tokenLine, str);
                loc.data.methods.push_back(str);
            }
            break ;
        default:
            break ;
    }
}

void    Config::parseLocation(t_ServerData &serv, std::string &content, std::string tokenLine)
{
    t_Location  loc = default_location_values;
    size_t      range[2];
    size_t      pos;

    range[0] = content.find("location");
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
    {
        range[1] = content.find("}");
        if (range[0] == std::string::npos || range[0] > range[1])
            break ;
        if (!(pos = findToken(content, range, static_cast<e_TokenType>(i))))
            continue ;
        else
            assignToken(loc, content, pos, static_cast<e_TokenType>(i));
    }
    eraseLine(content, tokenLine);
    eraseLine(content, "{");
    eraseLine(content, "}");
    serv.locations.push_back(loc);
}

void    Config::assignToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    std::string tokenLine = "default token line";
    std::string str = "UNDEFINED";
    int         nb = -1;

    tokenLine = getTokenLine(content, _Tokens[type], pos);
    if (type != LOCATION)
    {
        eraseLine(content, tokenLine);
        sanitizeLine(tokenLine);
    }
    switch (type)
    {
        case HOST:
            serv.host = tokenLine;
            break ;
        case LISTEN:
            serv.port = tokenLine;
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
            serv.autoindex = (tokenLine == "ON" || tokenLine == "on");
            break ;
        case ERROR_PAGE:
            nb = getNb(tokenLine, _Tokens[type]);
            str = getStr(tokenLine, _Tokens[type]);
            serv.error_pages.insert(std::make_pair(nb, str));
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
            serv.client_max_body_size = atoll(tokenLine.c_str());
            break ;
        case METHODS:
            for (int i = 0; !tokenLine.empty(); i++)
            {
                str = getStr(tokenLine, _Tokens[METHODS]);
                eraseLine(tokenLine, str);
                serv.methods.push_back(str);
            }
            break ;
        case LOCATION:
            parseLocation(serv, content, tokenLine);
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
    int             tokensFound = 0;

    if (trim.empty())
        return _servers.push_back(default_server_values);
    _servers.pop_back();    // rm default
    while (!trim.empty())
    {

        tokensFound = 0;
        serv = getDefaultServ(0);
        for (int i = 0; i < TOKEN_TYPE_COUNT + 1; i++)
        {
            while (1)
            {
                reset(serv, trim, servPos, servRange[0], servRange[1]);
                if (servRange[0] == std::string::npos)
                    break ;
                if (!(servPos = findToken(trim, servRange, static_cast<e_TokenType>(i))))
                {
                    if (i == LOCATION)
                        assignDefaultToken(serv, trim, servPos, i);
                    break ;
                }
                else
                {
                    tokensFound++;
                    assignToken(serv, trim, servPos, i);
                }
            }
        }
        if (!tokensFound)
            break ;
        _servers.push_back(serv);
        size_t to = trim.find('}');
        trim.erase(0, to + 1);
        _nbServers++;
    }
    std::cout <<BLUE <<  trim << RESET << '\n';
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
