#include "Debug.hpp"

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

    //  CLEAN FILE
    file.open("Debug.log");    
    if (!file.is_open())
    {
        std::cerr << "Debug file's creation failed\n";
        return ;
    }
    header << "Creation time : " << t << '\n'
        << "//////////////////////////\n"
        << "//  WEBSERV DEBUG LOG   //\n"
        << "//////////////////////////\n" << std::endl;
    file << header.str();
    header.clear();
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
