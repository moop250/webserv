#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"

//template <typename Token>
class Location
{
    protected:
        std::string                         _path;
        std::map<std::string, std::string>  _cgi;   // extension:path
        std::map<int, std::string>          _error_pages;  // nb:path
        std::vector<std::string>            _methods;
        std::string                         _root;
        std::string                         _index;
        std::string                         _upload_storage;
        size_t                              _client_max_size;
        bool                                _autoindex;
    public:
        std::string foo;
        Location();
        Location(t_Location loc);
        Location(t_ServerData serv);
        Location(const Location &);
        ~Location();

        Location    &operator=(const Location &);
    
        virtual bool    has(e_TokenType type = TOKEN_TYPE_COUNT);
    //    virtual bool        has(Token token);
    //    virtual Token       attribut(std::string tokenType, int member = 0);    //  find through a switch case after levenstein

        //  getters
        std::string path() const;
    
        virtual std::map<std::string, std::string> cgi() const;
        virtual std::map<int, std::string>          errorPages() const;
    
        virtual std::vector<std::string>    methods() const;
    
        virtual std::string root() const;
        virtual std::string index() const;
        virtual std::string storage() const;
        virtual size_t      clientSize() const;
        virtual bool        autoindex() const;

    //    Token       errorPageContent();
};

//template <typename Token>
std::ostream &operator<<(std::ostream &stream, const Location &);

//template <typename Token>
class Server : public Location
{
    private:
        std::vector<Location>   _locations;
        std::string             _host;
        std::string             _port;
        std::string             _server_name;
    public:
        Server();
        Server(t_ServerData serv);
        Server(const Server &);
        ~Server();

        Server  &operator=(const Server &);
    
        bool    has(e_TokenType type = TOKEN_TYPE_COUNT);
    //    Token   attribut(std::string token, int member = 0);    //  same

        //  getters
        std::string             host() const;
        std::string             port() const;
        std::string             name() const;

        Location                location(int at = 0) const;
        std::vector<Location>   locations() const;
    
    //    Token                   errorPageContent();
};

std::string tostring(int n);

#endif