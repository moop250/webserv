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
    bool        _isValid;
    bool        _isLocation;
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
        RequestServer(bool def=false);
        RequestServer(Config config);
       // RequestServer(Config config, const std::string &name, const std::string &port, const std::string &locPath);
        RequestServer(Config, std::string name, std::string port, std::string locPath);
        RequestServer(const RequestServer &);
        ~RequestServer();

        RequestServer   &operator=(const RequestServer &);

        void            setToken(t_ServerData serv, e_TokenType type);
        void            setToken(t_Location loc, e_TokenType type);
        bool            check(Config config, size_t portId, size_t nameId, size_t locId, std::string locPath);
//        void            setToken(std::string str, e_TokenType type);
        bool            undefined(e_TokenType);
        bool            has(e_TokenType);

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
        RequestServer                       defaultServ() const;

};

std::ostream    &operator<<(std::ostream &stream, const RequestServer &rs);

#endif