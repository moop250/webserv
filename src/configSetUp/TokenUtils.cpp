#include "Config.hpp"

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

    while (line[here] && !isspace(line[here]))
        here++;
    return here;
}

size_t  getNb(std::string line, std::string token)
{
    (void)token;
    size_t  nb = 0;
    std::string extract = line.substr(0, findNextSpace(line, nb));

    nb = atoll(extract.c_str());
    return (nb);
}

std::string getStr(std::string &line, std::string token)
{
    size_t  start = 0, end = 0;
    std::string str = "";

    (void)end;
    while (str[start] && !isalpha(line[start]))
        start++;
    str = line.substr(start, findNextSpace(line, start));
    return str;
}