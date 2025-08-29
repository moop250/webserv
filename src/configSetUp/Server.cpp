# include "Server.hpp"

Server::Server() : Location(),
    _host("UNDEFINED"), _port("UNDEFINED"), _server_name("UNDEFINED")
{
    return ;
}

Server::Server(t_ServerData serv) : Location(serv)
{
    _host = serv.host;
    _port = serv.port;
    _server_name = serv.server_name;
    _errorPages = ErrorPages(serv.error_pages);
    for (std::vector<t_Location>::iterator i = serv.locations.begin(); i != serv.locations.end(); i++)
        _locations.push_back(Location(*i));
    return ;
}

Server::Server(const Server &s) : Location(s)
{
    _host = s.host();
    _port = s.port();
    _server_name = s.name();
    _locations = s.locations();
    return ;
}

Server::~Server() { }

Server    &Server::operator=(const Server &s)
{
    if (this != &s)
        *this = s;
    return *this;
}

bool    Server::undefined(e_TokenType type)
{
    (void)type;
    return false;
}

bool    Server::has(e_TokenType type)
{
    return ;
}

bool    Server::has(std::string token, e_TokenType type)
{
    int    general = TOKEN_TYPE_COUNT;

    switch (type)
    {
        case TOKEN_TYPE_COUNT:
            while (general-- > 0)
                if (has(token, static_cast<e_TokenType>(general)))
                    return true;
            return (false);
        case LISTEN:
            if (token == _port)
                return true;
            return false;
        case HOST:
            if (token == _host)
                return true;
            return false;
        case SERVER_NAME:
            if (token == _server_name)
                return true;
            return false;
        case ROOT_PATH:
            if (token == _root)
                return true;
            return false;
        case HTLM_INDEX:
            if (token == _index)
                return true;
            return false;
        case AUTOINDEX:
            if (token == "ON" || "on")
                return true;
            return false;
        case ERROR_PAGE:
            if (_errorPages.find(static_cast<RequestError>(atoi(token.c_str()))))
                return true;
            return false;
        case UPLOAD_STORAGE:
            if (token == _upload_storage)
                return true;
            return false;
        case CGI_DATA:
            if (token == _cgi.find(token)->second || token == _cgi.find(token)->first)
                return true;
            return false;
        case METHODS:
            for (std::vector<std::string>::iterator i = _methods.begin(); i != _methods.end(); i++)
                if (token == *i)
                    return true;
            return false;
        case LOCATION:
            if (!_locations.empty())
                return true;
            return false;
        default:
            break;
    }
    return false;
}

int     find(std::string token, e_TokenType type)
{
    return (0);
}

std::string Server::host() const { return _host; }

std::string Server::port() const { return _port; }

std::string Server::name() const { return _server_name; }

Location    Server::location(int at) const {
    try {
        return _locations.at(at);
    } catch(std::exception &e) {
        throw "Out of bounds";
        std::cerr << e.what() << std::endl;
    }
}

std::vector<Location>   Server::locations() const { return _locations; }

//void    *Server::locAttribut(e_TokenType type) const {
//    switch (type)
//    {
//        case ROOT_PATH:
//            ///
//            break ;
//        case HTLM_INDEX:
//            //
//            break ;
//        case AUTOINDEX:
//            //
//            break ;
//        case ERROR_PAGE:
//            // 
//            break ;
//        case CGI_DATA:
//            //  
//            break ;
//        case UPLOAD_STORAGE:
//            //
//            break ;
//        case CLIENT_MAX_BODY_SIZE:
//            //
//            break ;
//        case METHODS:
//            // 
//            break ;
//    }
//    return NULL;
//}