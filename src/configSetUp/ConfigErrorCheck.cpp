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

static bool isFormatValid(int type, std::string content, std::string line)
{
   // std::string line = content;
//
   // (void)line;
    return 1;
}

static bool    isTokenValid(int type, std::string content, std::string line)
{
    size_t  found = 0;
    const char *tokens[TOKEN_TYPE_COUNT] = {
        "<host>", "<listen>", "<server_name>", "<root>",
        "<html_index>", "<autoindex>", "<error_page>", "<upload_storage>",
        "<cgi_ext>", "<cgi_path>", "<client_max_body_size>", "<allow>",
        "location",
    };

    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        if ((found = line.find(tokens[i])) != std::string::npos)
            break ;
    if (found == LOCATION || line.find("server") != std::string::npos)
        return 1;
    return 0;
}

static bool    isEndLineValid(int type, std::string content, std::string line)
{
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

//bool    ConfigError::foo()
//{
//    return KO;
//}

bool    ConfigError::checkContent()
{
   // bool    (*ConfigError::checkers[5])() = {
   //     foo, fii, faa, fuu, foo
   // };

    if (_content.empty())
        return 0;

}
