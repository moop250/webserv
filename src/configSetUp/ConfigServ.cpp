#include "Config.hpp"

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
            str = getStr(tokenLine, _Tokens[type]);
            eraseLine(tokenLine, str);
            nb = atoi(str.c_str());
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
        return _servers.push_back(getDefaultServ(0));
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
                //    std::cout << "Token found : " << _Tokens[i] << '\n';
                    tokensFound++;
                    assignToken(serv, trim, servPos, i);
                }
            }
        }
      //  std::cout << trim << std::endl;
        if (!tokensFound)
            break ;
        _servers.push_back(serv);
        size_t to = trim.find('}');
        trim.erase(0, to + 1);
        _nbServers++;
    }
    std::cout <<BLUE <<  trim << RESET << '\n';
}
