#include "ConfigError.hpp"

int    getLineType(std::string line)
{
    size_t      pos = 0;
    static const char *tokens[7] = {
        "location", "server", "{", "}", "#", "<", ""
    };
    
    std::cout << "Line is ; " << line << '\n';
    for (int type = 0; type < LINE_EMPTY; type++)
    {
        pos = line.find(tokens[type]);
        if (pos != std::string::npos)
            return (type);
    }
    return LINE_EMPTY;
}

std::string suggsestToken(std::string tokens[TOKEN_TYPE_COUNT], std::string line)
{
    return ("YEYEYEYEYE");
}

void    ConfigError::explicitTheError()
{
    switch (_error)
    {
        case ERROR_BRACKET:
            std::cerr << "Bracket bad\n";
            break ;
        case ERROR_NB_SERVER:
            std::cerr << "No server\n";
            break ;
        case ERROR_LINE_FORMAT:
            switch (_fmtError)
            {
                case FMT_EOF:
                    std::cerr << "fmt eof\n";
                    break ;
                case FMT_TOKEN:
                    _suggsestedToken = suggsestToken(_Tokens, _errorLine);
                    std::cerr << "Did you mean : "
                        << GREEN << _suggsestedToken << "?" << RESET << std::endl;
                    break ;
                case FMT_BRACKET:
                    std::cerr << "fmt bracket\n";
                    break ;
                default:
                    break ;
            }
            break ;
        case ERROR_TOKENS:
            std::cerr << "token error, token needed\n";
            break ;
        default:
            break ;
    }
    return ;
}

//  lines check
bool    ConfigError::eof(std::string line)
{
    switch (_line)
    {
        case LINE_LOC:
            break ;
        case LINE_SERV:
            break ;
        case LINE_BRACK1:
            break ;
        case LINE_BRACK2:
            break ;
        case LINE_COMMENT:
            break ;
        case LINE_TOK:
            break ;
        case LINE_EMPTY:
            break ;
        default:
            break ;
    }
    return OK;
}

bool    ConfigError::token(std::string line)
{
    switch (_line)
    {
        case LINE_LOC:
            break ;
        case LINE_SERV:
            break ;
        case LINE_BRACK1:
            break ;
        case LINE_BRACK2:
            break ;
        case LINE_COMMENT:
            break ;
        case LINE_TOK:
            break ;
        case LINE_EMPTY:
            break ;
        default:
            break ;
    }
    return OK;
}

bool    ConfigError::bracket(std::string line)
{
    switch (_line)
    {
        case LINE_LOC:
            break ;
        case LINE_SERV:
            break ;
        case LINE_BRACK1:
            break ;
        case LINE_BRACK2:
            break ;
        case LINE_COMMENT:
            break ;
        case LINE_TOK:
            break ;
        case LINE_EMPTY:
            break ;
        default:
            break ;
    }
    return OK;
}

bool    ConfigError::foo(std::string line)
{
    switch (_line)
    {
        case LINE_LOC:
            break ;
        case LINE_SERV:
            break ;
        case LINE_BRACK1:
            break ;
        case LINE_BRACK2:
            break ;
        case LINE_COMMENT:
            break ;
        case LINE_TOK:
            break ;
        case LINE_EMPTY:
            break ;
        default:
            break ;
    }
    return OK;
}

bool ConfigError::checkBrackets()
{
    int open = 0, close = 0;
    size_t  len = _content.length();

    std::cout << "In check brackets\n";
    for (size_t i = 0; i < len; i++)
    {
        if (_content[i] == '{')
            open++;
        if (_content[i] == '}')
            close++;
    }
    if (open != close)
        _isValid = 0;
    return OK;
}

bool    ConfigError::checkNbServers()
{
    std::cout << "In nb servers check\n";
    if (_nbServers <= 0)
        return KO;
    return OK;
}

bool    ConfigError::checkTokens()
{
    t_ServerData    serv;

    std::cout << "In checktokens\n";
    for (int i = 0; i < _nbServers; i++)
    {
        serv = _servers.at(i);
        if (serv.host == "UNDEFINED" || serv.port == "UNDEFINED")
            return (KO);    //  add here if more imperative tokens
    }
    return (OK);
}

bool    ConfigError::checkLinesFormat()
{
    std::stringstream   content;

    content << _content;
    while (std::getline(content, _errorLine))
    {
        _fmtError = 0;
        for (; _fmtError < FMT_FOO; _fmtError++)
            if ((this->*lineCheckers[_fmtError])(_errorLine))
                break ;
        if (_fmtError != FMT_FOO)
            return (KO);
    }
    std::cout << "In checklineformat\n"; 
    return (KO);
}

bool    ConfigError::checkConfig()
{
    bool    status;

    _error = 0;
    if (_content.empty())
        Error("empty file");
    for (; _error < CONFIG_CHECKERS; _error++)
        if (!(status = (this->*checkers[_error])()))
            break ;
    if (_isValid)
        return OK;
    Error(errors[_error].c_str());
    explicitTheError();
    return (KO);
}
