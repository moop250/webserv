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
    locId = 0;
    for (std::vector<Location>::iterator i = serv.locations().begin(); i != serv.locations().end(); i++, locId++)
        if (i->path() == locPath)
            break ;
    Location    loc(serv.location(locId));

    _isValid = true;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
        if (!serv.undefined(static_cast<e_TokenType>(i)))
            setToken(serv, static_cast<e_TokenType>(i));
    if (!locPath.empty())
        for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
            if (!serv.location(locId).undefined(static_cast<e_TokenType>(i)))
                setToken(loc, static_cast<e_TokenType>(i));
    _methods = config.getServerData(portId).methods;
    if (!config.getServerData(portId).locations.at(locId).data.methods.empty())
        _methods = config.getServerData(portId).locations.at(locId).data.methods;
    _cgi = config.getServerData(portId).cgi;
    if (!config.getServerData(portId).locations.at(locId).data.cgi.empty())
        _cgi = config.getServerData(portId).locations.at(locId).data.cgi;
    if (loc.clientSize() != 1)
        _clientBodySize = loc.clientSize();
    std::map<std::string, std::string> cgi = config.getServerData(portId).cgi;
    _cgi = cgi;
    _errorPages = ErrorPages(config.getServerData(portId).error_pages);
}

RequestServer::RequestServer(const RequestServer &serv)
{
    *this = serv;
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
          //  for (std::map<std::string, std::string>::iterator i = _cgi.begin(); i != _cgi.end(); i++)
          //  {
          //      _cgi.insert(std::make_pair(i->first, i->second));
          //      std::cout << "first : " << i->first << " second : " << i->second << std::endl; 
            //}
            _cgi = serv.cgi();
            break;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = serv.clientSize();
            break;
        case METHODS:
        //   for (std::vector<std::string>::iterator i = serv.methods().begin(); i != serv.methods().end();i++)
        //       _methods.push_back(*i);
            _methods = serv.methods();
         //   for (std::vector<std::string>::iterator i = _methods.begin(); i != _methods.end();i++)
         //       std::cout << "Method in loc : " << *i << std::endl;

            break;
        case REDIRECT:
            _redirect = serv.redirect();
            break ;
        default:
            break;
    }
}

void    RequestServer::setToken(Location loc, e_TokenType type)
{
    if (loc.undefined(type))
        return ;
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
         //   for (std::map<std::string, std::string>::iterator i = _cgi.begin(); i != _cgi.end(); i++)
         //   {
         //       _cgi.insert(std::make_pair(i->first, i->second));
         //       std::cout << "first : " << i->first << " second : " << i->second << std::endl; 
         //   }
            _cgi = loc.cgi();
            break ;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = loc.clientSize();
            break;
        case METHODS:
           // _methods = loc.methods();
            for (std::vector<std::string>::iterator i = loc.methods().begin(); i != loc.methods().end();i++)
                _methods.push_back(*i);
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
    if (this != &srv) {
        _isValid = srv._isValid;
        _cgi = srv._cgi;
        _methods = srv._methods;
        _errorPages = srv._errorPages;
        _host = srv._host;
        _port = srv._port;
        _serverName = srv._serverName;
        _root = srv._root;
        _index = srv._index;
        _storage = srv._storage;
        _redirect = srv._redirect;
        _clientBodySize = srv._clientBodySize;
        _autoindex = srv._autoindex;
    }
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

    for (size_t i = 0; i < rs.methods().size(); i++)
        stream << " " << rs.methods().at(i);
    stream << '\n';
    stream << "Host           : ";
    stream << rs.host();
    stream << '\n' << "Listeners      : ";
    stream << rs.port();
    stream << "\n";
    stream << "CGI             : \n";
  //  return stream;
    std::map<std::string, std::string> mprint = rs.cgi();
   if (rs.cgi().empty())
       stream << RED << "\t\t\tUNDEFINED\n" << RESET;
   else
       for (std::map<std::string, std::string>::const_iterator j = mprint.begin(); j != mprint.end(); j++)
          stream << "\textension :" << j->first << " <==> path : " << j->second << '\n';
    stream << "error pages       : ";
    for (int j = 0; j < OTHER; j++)
        stream <<  rs.errorPages().content(j);
         //  stream << "\t\t" << j->first << " <==> " << j->second << '\n';
    stream << '\n';
    return (stream);
}