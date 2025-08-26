#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"

template <typename Token>
class Location
{
    protected:
        std::string                             _path;
        std::vector<std::tuple<std::string>>    _cgi;   // extension:path
        std::vector<std::tuple<std::string>>    _error_pages;
        std::vector<std::string>                _methods;
        std::string                             _root;
        std::string                             _index;
        std::string                             _upload_storage;
        size_t                                  _client_max_size;
        bool                                    _autoindex;
    public:
        Location();
        Location(t_Location loc);
        Location(const Location &);
        ~Location();

        Location    &operator=(const Location &);
    
        bool        has(Token token);
        Token       attribut(std::string tokenType, int member = 0);    //  find through a switch case after levenstein

        Token       errorPageContent();
};

template <typename Token>
class Server : public Location
{
    private:
        std::vector<Location>                   _locations;
        std::string                             _host;
        std::string                             _port;
        std::string                             _server_name;
    public:
        Server();
        Server(t_ServerData serv);
        Server(const Server &);
        ~Server();

        Server  &operator=(const Server &);
    
        bool    has(Token token);
        Token   attribut(std::string token, int member = 0);    //  same

        Token   errorPageContent();
};

#endif