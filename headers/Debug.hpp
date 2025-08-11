#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <ostream>
# include <fstream>
# include <string>
# include <iostream>
# include <sstream>
# include "Colors.hpp"
# include <time.h>

class Debug
{
    public:
        Debug();
        ~Debug();
        
        void    append(const char *msg);
        std::ofstream   file;
};

static std::string  getTime()
{
    time_t          now = time(0);
    struct tm       tstruct = *localtime(&now);
    char            buf[80];
    std::string     msg = "";

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    msg += buf;
    return (msg);
}

Debug::Debug()
{
    std::stringstream   header;
    const std::string   t = getTime();

    file.open("Debug.log");    
    if (!file.is_open())
    {
        std::cerr << "Debug file's creation failed\n";
        return ;
    }
    header << WHITE << "Creation time : " << t << '\n' << YELLOW
        << "//////////////////////////\n"
        << "//  WEBSERV DEBUG LOG   //\n"
        << "//////////////////////////\n" << RESET << std::endl;
    file << header.str();
    return ;
}

Debug::~Debug()
{
    if (file.is_open())
        file.close();
    return ;
}

void    Debug::append(const char *msg)
{
    if (!file.is_open())
        return ;
    file << msg << '\n';
    return ;
}


#endif