#ifndef REQUESTSERVER_HPP
# define REQUESTSERVER_HPP

# include "StdLibs.hpp"
# include "ErrorPages.hpp"
# include "Config.hpp"

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
    size_t                              _clientBodySize;    //both
    bool                                _autoindex; // both
    public:
        RequestServer();
        RequestServer(Config, std::string, std::string);
        RequestServer(const RequestServer &);
        ~RequestServer();

        RequestServer   &operator=(const RequestServer &);
};


#endif