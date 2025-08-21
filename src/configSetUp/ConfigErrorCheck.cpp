#include "ConfigError.hpp"

bool    getLineType(std::string line)
{
    //  loc type
    //  serv type
    //  token  type
    //  bracket type
    //  commented type
    //  empty type
    return 1;
}

std::string suggsestToken(std::string tokens[TOKEN_TYPE_COUNT], std::string line)
{
    return ("YEYEYEYEYE");
}

enum e_config_errors
{
    ERROR_BRACKET,
    ERROR_NB_SERVER,
    ERROR_LINE_FORMAT,
    ERROR_TOKENS,
};

enum e_format_errors
{
    FMT_EOF,
    FMT_TOKEN,
    FMT_BRACKET,
    FMT_FOO
};
void    ConfigError::explicitTheError()
{
    switch (_error)
    {
        case ERROR_BRACKET:
            break ;
        case ERROR_NB_SERVER:
            break ;
        case ERROR_LINE_FORMAT:
            switch (_fmtError)
            {
                case FMT_EOF:
                    break ;
                case FMT_TOKEN:
                    _suggsestedToken = suggsestToken(_Tokens, _errorLine);
                    std::cerr << "Did you mean : "
                        << GREEN << _suggsestedToken << "?" << RESET << std::endl;
                    break ;
                case FMT_BRACKET:
                    break ;
                default:
                    break ;
            }
            break ;
        case ERROR_TOKENS:
            break ;
        default:
            break ;
    }
    return ;
}

//  lines check
bool    ConfigError::eof(std::string line)
{
    return KO;
}

bool    ConfigError::token(std::string line)
{
    return KO;
}

bool    ConfigError::bracket(std::string line)
{
    return KO;
}

bool    ConfigError::foo(std::string line)
{
    return KO;
}

bool ConfigError::checkBrackets()
{
    int open = 0, close = 0;
    size_t  len = _content.length();

    for (size_t i = 0; i < len; i++)
    {
        if (_content[i] == '{')
            open++;
        if (_content[i] == '}')
            close++;
    }
    if (open != close)
        return KO;
    return OK;
}

bool    ConfigError::checkNbServers()
{
    if (_nbServers <= 0)
        return KO;
    return OK;
}

bool    ConfigError::checkTokens()
{
    std::cout << "In checktokens\n";
    return (KO);
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

    if (_content.empty())
        Error("empty file");
    for (_error = 0; _error < CONFIG_CHECKERS; _error++)
        if ((status = (this->*checkers[_error])()))
            continue ;
    if (_isValid)
        return OK;
    Error(errors[_error].c_str());
    explicitTheError();
    std::cout << "Config Checked\n";
    return (KO);
}
