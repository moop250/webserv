#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>

class Location
{
    private:
        std::string _path;
    public:
        Location(/* args */);
        ~Location();
        attribut(std::string token);    //  find through a switch case after levenstein
};

class Server
{
    private:
        std::string _host;
    public:
        Server();
        ~Server();
        attribut(std::string token);    //  same
};

#endif