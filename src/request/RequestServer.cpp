#include "RequestServer.hpp"

RequestServer::RequestServer() {
    _isValid = false;
    _cgi.insert(std::make_pair("UNDEFINED", "UNDEFINED"));
    _methods.push_back("UNDEFINED");
    _errorPages = ErrorPages();
    _host = "UNDEFINED";
    _port = "UNDEFINED";
    _serverName = "UNDEFINED";
    _root = "UNDEFINED";
    _index = "UNDEFINED";
    _storage = "UNDEFINED";
    _redirect = "UNDEFINED";
    _clientBodySize = 0;
    _autoindex = false;
}

RequestServer::RequestServer(Config config, std::string name, std::string port, std::string locPath)
{
    size_t portId = config.find(port, LISTEN);
    size_t nameId = config.find(name, SERVER_NAME);
    if (portId == std::string::npos || nameId != portId)
    {
        *this = RequestServer();
        std::cerr << RED << "ServerName or associeted port not found\n" << RESET;
        _isValid = false;
        return ;
    }
    size_t  locId = config.find(locPath, LOCATION_PATH);
    if (locId == std::string::npos && !locPath.empty() && portId != locId)
    {
        *this = RequestServer();
        std::cerr << RED << "Location path in server nb : " << portId
            << " not found\n" << RESET;
        _isValid = false;
        return ;
    }
    
    Server      serv(config.getServerData(portId));
    Location    loc(serv.location(locId));

    std::cout << "Constructor\n";
    std::cout << "redir : " << serv.redirect() << std::endl;
    std::cout << "redir in struct : " << config.getServerData(portId).redirect << std::endl;
    std::cout << "other : " << serv.index() << '\n';
    std::cout << "root : " << serv.root() << std::endl;
    std::cout << "size : " << serv.clientSize() << '\n';
    _isValid = true;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        if (!serv.undefined(static_cast<e_TokenType>(i)))
            setToken(serv, static_cast<e_TokenType>(i));
    return ;
    if (!locPath.empty())
        for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
            if (!serv.location(locId).undefined(static_cast<e_TokenType>(i)))
                setToken(loc, static_cast<e_TokenType>(i));
}

//template <typename integer>
//RequestServer::RequestServer(Config config, std::string locPath, integer port)
//{
//    size_t servId = config.find(tostring(port), SERVER_NAME);
//    if (servId == std::string::npos)
//    {
//        *this = RequestServer();
//        std::cerr << RED << "ServerName not found\n" << RESET;
//        _isValid = false;
//        return ;
//    }
//    size_t  locId = config.find(locPath, LOCATION_PATH);
//    if (locId == std::string::npos && !locPath.empty())
//    {
//        *this = RequestServer();
//        std::cerr << RED << "Location path in server nb : " << servId
//            << " not found\n" << RESET;
//        _isValid = false;
//        return ;
//    }
//    
//    Server      serv(config.getServerData(servId));
//    Location    loc(serv.location(locId));
//
//    _isValid = true;
//    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
//        if (!serv.undefined(static_cast<e_TokenType>(i)))
//            setToken(serv, static_cast<e_TokenType>(i));
//    if (!locPath.empty())
//        for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
//            if (!serv.location(locId).undefined(static_cast<e_TokenType>(i)))
//                setToken(loc, static_cast<e_TokenType>(i));
//}

RequestServer::RequestServer(const RequestServer &serv)
{
    (void)serv;
    return ;
}

RequestServer::~RequestServer() {}

void    RequestServer::setToken(Server serv, e_TokenType type)
{
    switch (type)
    {
        case HOST:
            _host = serv.host();
            break;
        case LISTEN:
            _port = serv.port();
            break;
        case SERVER_NAME:
            _serverName = serv.name();
            break;
        case ROOT_PATH:
            _root = serv.root();
            break ;
        case HTLM_INDEX:
            _index = serv.index();
            break;
        case AUTOINDEX:
            _autoindex = serv.autoindex();
            break;
        case ERROR_PAGE:
            _errorPages = serv.errorPages();
            break;
        case UPLOAD_STORAGE:
            _storage = serv.storage();
            break;
        case CGI_DATA:
            _cgi = serv.cgi();
            break;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = serv.clientSize();
            break;
        case METHODS:
            _methods = serv.methods();
            break;
        case REDIRECT:
            _redirect = serv.redirect();
        default:
            break;
    }
}

void    RequestServer::setToken(Location loc, e_TokenType type)
{
    switch (type)
    {
        case HOST:
            // nothing
            break;
        case LISTEN:
            // nothing
            break;
        case SERVER_NAME:
            //  nothing
            break;
        case ROOT_PATH:
            _root = loc.root();
            break ;
        case HTLM_INDEX:
            _index = loc.index();
            break;
        case AUTOINDEX:
            _autoindex = loc.autoindex();
            break;
        case ERROR_PAGE:
            _errorPages = loc.errorPages(); // check if push_back instead
            break ;
        case UPLOAD_STORAGE:
            _storage = loc.storage();
            break;
        case CGI_DATA:
            _cgi = loc.cgi();
            break;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = loc.clientSize();
            break;
        case METHODS:
            _methods = loc.methods();
            break;
        case REDIRECT:
            _redirect = loc.redirect();
            break ;
        default:
            break;
    }
}

RequestServer   &RequestServer::operator=(const RequestServer &srv)
{
    (void)srv;
    return *this;
}

bool            RequestServer::isValid() const { return _isValid; }

std::map<std::string, std::string>  RequestServer::cgi() const { return _cgi;}
std::vector<std::string>            RequestServer::methods() const { return _methods;}
ErrorPages                          RequestServer::errorPages() const { return _errorPages;}
std::string                         RequestServer::host() const { return _host;}
std::string                         RequestServer::port() const { return _port;}
std::string                         RequestServer::serverName() const { return _serverName;}
std::string                         RequestServer::root() const { 
    Color   c;
    if (_root == "UNDEFINED" || _root == c.red() + "UNDEFINED" + c.reset())
        return "";
    return _root;
    }
std::string                         RequestServer::index() const {
    Color   c;
    if (_index == "UNDEFINED" || _index == c.red() + "UNDEFINED" + c.reset())
        return "";
    return _index;
    }
std::string                         RequestServer::storage() const {
    Color   c;
    if (_storage == "UNDEFINED" || _storage == c.red() + "UNDEFINED" + c.reset())
        return "";
    return _storage;
    }
std::string                         RequestServer::redirect() const {
    Color   c;
    if (_redirect == "UNDEFINED" || _redirect == c.red() + "UNDEFINED" + c.reset())
        return "";
    return _redirect;
}
size_t                              RequestServer::clientSize() const { return _clientBodySize;}
bool                                RequestServer::autoindex() const { return _autoindex;}

std::ostream    &operator<<(std::ostream &stream, const RequestServer &rs)
{
    stream  << "Server name    : " << rs.serverName() << '\n'
            << "Root path      : " << rs.root() << '\n'
            << "Index file     : " << rs.index() << '\n'
            << "Auto index     : " << (rs.autoindex() ? "On" : "OFF") << '\n'
            << "Upload storage : " << rs.storage() << '\n'
            << "Max client siz : " << rs.clientSize() << '\n'
            << "Redirection    : " << rs.redirect() << std::endl; 
    stream << "Methods        : ";
    for (std::vector<std::string>::iterator i = rs.methods().begin(); i != rs.methods().end(); i++)
        stream << " m :" << *i;
    stream << '\n';
    stream << "Host           : ";
    stream << rs.host();
    stream << '\n' << "Listeners      : ";
    stream << rs.port();
    stream << "\n";
    stream << "CGI             : \n";
    if (rs.cgi().empty())
        stream << RED << "\t\t\tUNDEFINED\n" << RESET;
    else
        for (std::map<std::string, std::string>::iterator j = rs.cgi().begin(); j != rs.cgi().end(); j++)
            stream << "\textension :" << j->first << " <==> path : " << j->second << '\n';
    stream << "error pages       : ";
    if (rs.errorPages().content(0).empty())
        stream << RED << "\t\tUNDEFINED" << RESET;
    else
        stream << "Content to long to print...\n";
   //     for (std::map<int, std::string>::iterator j = rs.errorPages(); j != rs.errorPages.end(); j++)
   //         stream << "\t\t" << j->first << " <==> " << j->second << '\n';
    stream << '\n';
    return (stream);
}