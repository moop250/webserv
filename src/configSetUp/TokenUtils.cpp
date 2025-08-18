#include "Config.hpp"

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
    if (content.find("location", rBegin) < rEnd - 10)
        rEnd = content.find("location", rBegin);
    else
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

void    sanitizeLine(std::string &line)
{
    int to_remove = 0;

    while (!isspace(line[to_remove]))
        to_remove++;
    while (isspace(line[to_remove]))
        to_remove++;
    line.erase(0, to_remove);
    return ;
}

std::string getTokenLine(const std::string &content, const std::string &token, size_t pos)
{
    size_t end = content.find(';', pos);

    if (token == "location")
        end = content.find('{', pos);
    if (end == std::string::npos)
        end = content.size();
    std::string line = content.substr(pos, end - pos);
    return line;
}

void eraseLine(std::string &content, const std::string &line)
{
    size_t  from = content.find(line);

    if (from == std::string::npos)
        return;    
    content.erase(from, line.length());
    if (from < content.size() && (content[from] == ';' || isspace(content[from])))
        content.erase(from, 1);
}

size_t  findNextSpace(std::string line, size_t &from)
{
    size_t  here = from;

    while (!isspace(line[here]))
        here++;
    return here;
}


size_t  getNb(std::string line, std::string token)
{
    size_t  nb = 0;
    std::string extract = line.substr(0, findNextSpace(line, nb));

    nb = atoll(extract.c_str());
    return (nb);
}

std::string getStr(std::string &line, std::string token)
{
    size_t  start = 0, end = 0;
    std::string str = "";

    while (str[start] && !isalpha(line[start]))
        start++;
    str = line.substr(start, findNextSpace(line, start));
    return str;
}


void    assignDefaultToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    (void)content;
    (void)pos;
     switch (type)
    {
        case HOST:
            serv.host = "UNDEFINED";
                break ;
        case LISTEN:
            serv.port ="UNDEFINED";
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
