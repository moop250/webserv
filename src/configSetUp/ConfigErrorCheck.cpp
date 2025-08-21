#include "ConfigError.hpp"

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

enum
{
    TOKEN_ERROR,
    EOF_ERROR,
    FMT_ERROR
};

void    explicitTheError(int error, std::string tokens[TOKEN_TYPE_COUNT], std::string content, std::string line)
{
    std::string     suggestion = suggsestToken(tokens, line);

    switch (error)
    {
        case TOKEN_ERROR:
            std::cerr << "Did you mean : "
                << GREEN << suggestion << "?" << RESET << std::endl;
        case EOF_ERROR:
            break ;
        case FMT_ERROR:
            break ;
        default:
            break ;
    }
    return ;
}

//  lines check
bool    ConfigError::eof(std::string line)
{
    std::cout << "In eof\n"; 
    return KO;
}

bool    ConfigError::token(std::string line)
{
    std::cout << "In tokens\n"; 

    return KO;
}

bool    ConfigError::bracket(std::string line)
{
    std::cout << "In brackets\n"; 
    return KO;
}

bool    ConfigError::foo(std::string line)
{
    std::cout << "In foo\n"; 
    return KO;
}

bool    ConfigError::checkNbServers()
{
    std::cout << "In checknbservers\n"; 
    return KO;
}

bool    ConfigError::checkLinesFormat()
{
    std::cout << "In checklineformat\n"; 
    return (KO);
}

bool    ConfigError::checkTokens()
{
    std::cout << "In checktokens\n"; 
    return (KO);
}

bool    ConfigError::checkConfig()
{
    bool    status;

    if (_content.empty())
        throw ConfigError::BadFileException();
    for (int i = 0; i < CONFIG_CHECKERS; i++)
    if ((status = (this->*checkers[i])()))
            continue ;
    std::cout << "Config Checked\n";
    return (KO);
}
