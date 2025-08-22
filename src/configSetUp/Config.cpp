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

Config::Config() {
    std::cerr << "You are not supposed to use config without any file to look after\n";
}

void    formatContent(std::string &buf)
{
    size_t            pos = 0;
    size_t            lastpos;
    static const char *tokens[3] = {
        "{", "}", ";"
    };

    for (int i = 0; i < 3; i++)
    {
        pos = 0, lastpos = 0;
        while ((pos = buf.find(tokens[i], lastpos)) != std::string::npos)
        {
            buf.insert(pos + 1, 1, '\n');
            lastpos = pos + 2;
        }
    }
}

Config::Config(std::string fileName, Debug &dfile) :
    _dfile(&dfile), _nbServers(0), _fileName(fileName)
{
    std::ifstream   readFile(fileName.c_str());
    std::string     buf;

    if (readFile.is_open())
    {
        while (std::getline(readFile, buf))
            _content.append(buf);
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
    formatContent(_content);
}

Config::Config(const Config &conf)
{
    this->_dfile = conf._dfile;
    this->_nbServers = conf._nbServers;
    this->_fileName = conf._fileName;
    this->_content = conf._content;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        this->_Tokens[i] = conf._Tokens[i];
    this->_servers = conf._servers;
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
            return (SIZE_MAX);
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
