#include "Config.hpp"

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
    t_Location  loc = getDefaultServ(1).locations.at(0);
    std::string path = "";
    size_t      range[2];
    size_t      pos;

    range[0] = content.find("location");
    path = content.substr(range[0], content.find('{', range[0]) - range[0] - 1);
    sanitizeLine(path);
    loc.path = path;
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

