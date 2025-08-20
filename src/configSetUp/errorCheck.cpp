#include "Config.hpp"

#ifndef KO
# define KO 0
# define OK 1
#endif

bool checkBracket(std::string content)
{
    int open = 0, close = 0;
    size_t  len = content.length();

    for (size_t i = 0; i < len; i++)
    {
        if (content[i] == '{')
            open++;
        if (content[i] == '}')
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
    return OK;
}

static bool isFormatValid(int type, std::string content, std::string line)
{
   // std::string line = content;
//
   // (void)line;
    return OK;
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
        return OK;
    return KO;
}

static bool    isEndLineValid(int type, std::string content, std::string line)
{
    return OK;
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

bool    Config::checkContent()
{
    std::string         line = "";
    std::stringstream   content;
    std::string         errors[3] = {
        "Token not valid",
        "Format not rexpected",
        "Empty server or location"
    };
    static bool         (*funcs[3])(int, std::string, std::string) = {
        isTokenValid,
        isEndLineValid,
        isFormatValid
    };

    if (_content.empty())
        Error("Directory or empty file was given");
    if (!checkBracket(_content))
        Error("Unmatching brackets : '{' or '}' expected");
    content << _content;
    while (std::getline(content, line))
    {
        int lineType = getLineType(line);
        for (int i = 0; i < 3; i++)
        {
            bool check = funcs[i](lineType, _content, line);
            if (check == KO)
            {
                std::cerr << "In configuration file :\n"
                    << ROSE << line << '\n' << RESET;
                Error(errors[i].c_str());
                explicitTheError(i, _Tokens, _content, line);
                return KO;
            }
        }
    }
    return OK; 
}