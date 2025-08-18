#include "Config.hpp"

#ifndef KO || OK
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

bool checkBadTokens(std::string content)
{
    std::string line = content;
    std::string tokens[TOKEN_TYPE_COUNT] = {
        "<>", "<>", "<>", "<>", "<>",
        "<>", "<>", "<>", "<>", "<>",
        "<>"
    };

    while (!line.empty())
    {
    //    line = getline(content);
        if (line.find("<") != std::string::npos || line.find(">") != std::string::npos)
        {
            //  s;
            return OK;
        }
        return KO;
    }
    return OK;
}

bool checkFormat(std::string content)
{
    std::string line = content;

    while (!line.empty())
    {
    //    line = getline(content);
        //  check
        return KO;
    }
    return OK;
}

bool    checkEmpty(std::string content)
{
    size_t  start = 0, end = 0;
    std::string server;

    while (start != std::string::npos)
    {
        //  check
        return KO;
    }
    return OK;
}

//    bool        checkBracket(std::string content);
//    bool        checkBadTokens(std::string content);
//    bool        checkFormat(std::string content);
//    bool        checkEmpty(std::string content);


bool    checkContent()
{
    std::string errors[4] = {
        "Unmatching brackets", "Token not valid",
        "Format not rexpected", "Empty server or location"
    };
    static bool (*funcs[4])(std::string) = {
        checkBracket,
        checkBadTokens,
        checkFormat,
        checkEmpty
    };

    for (int i = 0; i < 4; i++)
    {
        bool    check = funcs[i];
        if (check == KO)
        {
            Error(errors[i].c_str());
            return KO;
        }
    }
    return OK; 
}