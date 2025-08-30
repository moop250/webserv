#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "ErrorPages.hpp"
# include "Location.hpp"

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
    
        bool    has(e_TokenType type);
        bool    has(std::string token, e_TokenType = TOKEN_TYPE_COUNT);
        bool    undefined(e_TokenType type);

        int     find(std::string token, e_TokenType type);
        template <typename Token>
        Token   attribut(e_TokenType);

        //  getters
        std::string             host() const;
        std::string             port() const;
        std::string             name() const;
        
        Location                location(int at = 0) const;
        std::vector<Location>   locations() const;
        
    //    int                     f(std::string token, e_TokenType type = TOKEN_TYPE_COUNT);
};

std::string tostring(int n);

#endif