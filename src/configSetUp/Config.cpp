#include "Config.hpp"

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

Config::Config() :
    _valid(1), _errorLine("")
{
    std::cerr << "You are not supposed to use config without any file to look after\n";
}

Config::Config(std::string fileName, Debug &dfile) :
    _valid(1), _errorLine(""), _dfile(&dfile), _nbServers(0)
{
    std::ifstream   readFile(fileName.c_str());
    std::string     buf;

    if (readFile.is_open())
    {
        while (std::getline(readFile, buf))
        {
            _content.append(buf);
            _content.append("\n");
        }
    }
    else
    {
        _content = "";
        Error("File is empty or does not exist");
        return ;
    }
    _dfile->append(buf.c_str());
    initTokenMaps();
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
        return (getDefaultServ(0));
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

std::string Config::getErrorLine() const
{
    return _errorLine;
}

//bool    Config::isValid() const
//{
//    return _valid;
//}

size_t  Config::findToken(std::string &content, size_t range[2], e_TokenType i)
{
    size_t  pos = content.find(_Tokens[i], range[BEGIN]);

    if (pos == std::string::npos || pos <= range[BEGIN] || pos >= range[END])
        return (0);
    if (pos == std::string::npos)
        return (0);
    size_t  tmp = pos;
    while (tmp > 0 && (content[tmp] != ';' || content[tmp] != '{'))
    {
        if (content[tmp] == '#')
        {
            pos = tmp;
            while (pos < content.length() && (content[pos] != ';' && content[pos] != '}' && content[pos] != '{'))
                pos++;
            std::cout << "Line ignored :\n";
            std::cout << YELLOW << content.substr(tmp, pos - tmp + 1) << RESET << '\n';
            content.erase(tmp, pos - tmp);
            return (-1);
        }
        tmp--;
    }
    return (pos);
}

void    Config::sanitize()
{
    for (int i = 0; i < _nbServers; i++)
    {
        _servers.at(i).locations.pop_back();
        _servers.at(i).locations.erase(_servers.at(i).locations.begin());
    }
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
