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
    
        bool    has(e_TokenType type = TOKEN_TYPE_COUNT);
    //    Token   attribut(std::string token, int member = 0);    //  same

        //  getters
        std::string             host() const;
        std::string             port() const;
        std::string             name() const;
        
        Location                location(int at = 0) const;
        std::vector<Location>   locations() const;
        
        bool                    undefined();
        bool                    has(std::string token, e_TokenType = TOKEN_TYPE_COUNT);
};

std::string tostring(int n);

#endif