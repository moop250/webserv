#ifndef REQUESTSERVER_HPP
# define REQUESTSERVER_HPP

# include "StdLibs.hpp"
# include "ErrorPages.hpp"
# include "Config.hpp"
# include "Server.hpp"

//enum RequestServerTypes
//{
//    REQUEST_CGI,
//    REQUEST_METHOD,
//    REQUEST_ERRORPAGE,
//    REQUEST
//};

class RequestServer
{
    bool    _isValid;
    std::map<std::string, std::string>  _cgi;   // both
    std::vector<std::string>            _methods;   //  both
    ErrorPages                          _errorPages;    // both
    std::string                         _host;      //  serv
    std::string                         _port;      //  serv
    std::string                         _serverName;    // serv
    std::string                         _root;  //  both
    std::string                         _index;     // both
    std::string                         _storage;   // both
    std::string                         _redirect;
    size_t                              _clientBodySize;    //both
    bool                                _autoindex; // both
    public:
        RequestServer();
        RequestServer(Config, std::string name, std::string port, std::string locPath);
        RequestServer(const RequestServer &);
        ~RequestServer();

        RequestServer   &operator=(const RequestServer &);

        void            setToken(Server serv, e_TokenType type);
        void            setToken(Location loc, e_TokenType type);
//        void            setToken(std::string str, e_TokenType type);

        std::map<std::string, std::string>  cgi() const;
        std::vector<std::string>            methods() const;
        ErrorPages                          errorPages() const;
        std::string                         host() const;
        std::string                         port() const;
        std::string                         serverName() const;
        std::string                         root() const;
        std::string                         index() const;
        std::string                         storage() const;
        std::string                         redirect() const;
        size_t                              clientSize() const;
        bool                                autoindex() const;
        bool                                isValid() const;
};

std::ostream    &operator<<(std::ostream &stream, const RequestServer &rs);

#endif