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
//    _errorPages = ErrorPages(serv.error_pages);
//    if (serv.locations.size() > 0)
//        std::cout << serv.locations.at(0).path;
    this->_locations.push_back(Location());
    for (size_t i = 0; i < serv.locations.size(); i++)
    {
        this->_locations.push_back(Location(serv.locations.at(i)));
    }
   // for (std::vector<t_Location>::iterator i = serv.locations.begin(); i != serv.locations.end();)
   // {
   //     _locations.push_back(Location(*i));
   //     i++;
   // }
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
    Color   c;
    switch (type)
    {
        case LISTEN:
            if (_port == "UNDEFINED" || _port.empty() || _port == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case HOST:
            if (_host == "UNDEFINED" || _host.empty() || _host == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case SERVER_NAME:
            if (_server_name == "UNDEFINED" || _server_name.empty() || _server_name == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case ROOT_PATH:
            if (_root == "UNDEFINED" || _root.empty() || _root == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case HTLM_INDEX:
            if (_index == "UNDEFINED" || _index.empty() || _index == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case AUTOINDEX:
            if (!_autoindex)
                return true;
            return false;
        case ERROR_PAGE:
            if (_errorPages.content().empty())
                return true;
            return false;
        case UPLOAD_STORAGE:
            if (_upload_storage == "UNDEFINED" || _upload_storage.empty() || _upload_storage == c.red() + "UNDEFINED" + c.reset())
                return true;
            return false;
        case CGI_DATA:
            if (_cgi.empty())
                return true;
            return false;
        case METHODS:
            if (_methods.empty())
                return true;
            return false;
        case REDIRECT:
            if (_redirect.empty())
                return true;
            return false;
        case LOCATION:
            if (_locations.empty())
                return true;
            return false;
        case LOCATION_PATH:
            std::cerr << "Logic for loc path not implemented\n";
            return (false);
        default:
            break;
    }
    return false;
}

bool    Server::has(e_TokenType type)
{
    if (undefined(type))
        return (false);
    return (true);
}

bool    Server::has(std::string token, e_TokenType type)
{
    int    general = TOKEN_TYPE_COUNT;

    std::cout << "Looking for token : " << token;
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
        case REDIRECT:
            if (token == _redirect)
                return true;
            return false;
        case LOCATION:
            if (!_locations.empty())
                return true;
            return false;
        case LOCATION_PATH:
            for (std::vector<Location>::iterator i = _locations.begin(); i != _locations.end(); i++)
                if (i->path() == token)
                    return (true);
            return (false);
        default:
            break;
    }
    return false;
}

template <typename Token>
Token   Server::attribut(e_TokenType type)
{
    switch (type)
    {
        case LISTEN:
            return _port;
        case HOST:
            return _host;
        case SERVER_NAME:
            return _server_name;
        case ROOT_PATH:
            return _root;
        case HTLM_INDEX:
            return _index;
        case AUTOINDEX:
            return _autoindex;
        case ERROR_PAGE:
            return _errorPages;
        case UPLOAD_STORAGE:
            return _upload_storage;
        case CGI_DATA:
            return _cgi;
        case METHODS:
            return _methods;
        case LOCATION:
            return _locations;
        default:
            break;
    }
    return ("UNDEFINED");
}

int     find(std::string token, e_TokenType type)
{
    // switch (type)
    // {
        // case HOST:
            // if (token == _HAS_CHAR16_T_LANGUAGE_SUPPORT)
                // break;
        // case LISTEN:
            // /* code */
            // break;
        // case SERVER_NAME:
            // /* code */
            // break;
        // case ROOT_PATH:
            // /* code */
            // break;
        // case HTLM_INDEX:
            // /* code */
            // break;
        // case AUTOINDEX:
            // /* code */
            // break;
        // case ERROR_PAGE:
            // /* code */
            // break;
        // case UPLOAD_STORAGE:
            // /* code */
            // break;
        // case CGI_DATA:
            // /* code */
            // break;
        // case CLIENT_MAX_BODY_SIZE:
            // /* code */
            // break;
        // case METHODS:
            // /* code */
            // break;
        // case LOCATION:
            // /* code */
            // break;
        // default:
            // break;
    // }
    (void)token;
    (void)type;
    return (0);
}

std::string Server::host() const { return _host; }

std::string Server::port() const { return _port; }

std::string Server::name() const { return _server_name; }

Location    Server::location(int at) const {
    if (_locations.size() < static_cast<size_t>(at))
        return _locations.at(0);
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

std::ostream    &operator<<(std::ostream &stream, const Server &serv)
{
    stream << CYAN << "SERVER CONTENT\n";
    stream << "server name : " << serv.name() << '\n';
    stream << "client size : " << serv.clientSize() << '\n'
        << "Redirection : " << serv.redirect() << '\n'
        << "Host : " << serv.host() << '\n'
        << "methods : ";
    for (size_t i = 0; i < serv.methods().size(); i++)
        stream << " " << serv.methods().at(i);
    stream << "\nCgi : \n";
//    for (std::map<std::string, std::string>::iterator i = serv.cgi().begin(); i != serv.cgi().end(); i++)
//        stream << "extention : " << i->first << " path : " << i->second << '\n';
    stream << RESET;
    return stream;
 }