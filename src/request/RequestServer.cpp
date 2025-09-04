#include "RequestServer.hpp"

RequestServer::RequestServer() {
    _isValid = false;
  //  _cgi.insert(std::make_pair("UNDEFINED", "UNDEFINED"));
  //  _methods.push_back("UNDEFINED");
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

static bool    check(Config config, size_t portId, size_t nameId, size_t locId, std::string locPath)
{
    if (portId == std::string::npos || nameId != portId)
    {
        std::cerr << RED << "ServerName or associeted port not found\n" << RESET;
        return false;
    }
    if ((locId == std::string::npos && !locPath.empty() && portId != locId)
        || locId >= config.getServerData(portId).locations.size())
    {
        std::cerr << RED << "Location path in server nb : " << portId
            << " not found\n" << RESET;
        return false;
    }
    return true;
}

RequestServer::RequestServer(Config config, std::string name, std::string port, std::string locPath)
{
    size_t portId = config.find(port, LISTEN);
    size_t nameId = config.find(name, SERVER_NAME);
    size_t  locId = config.find(locPath, LOCATION_PATH);
    
    if (!check(config, portId, nameId, locId, locPath))
    {
        _isValid = false;
        *this = RequestServer();
        return ;
    }

    t_ServerData    s = config.getServerData(portId);
    
    locId = 0;
    for (std::vector<t_Location>::iterator i = s.locations.begin(); i != s.locations.end(); i++)
    {
        if (i->path == locPath)
            break ;
        locId++;
    }
    t_Location      l = s.locations.at(locId);

    _isValid = true;
    if (!locPath.empty())
       for (int i = 0; i < LOCATION; i++)
            setToken(l, static_cast<e_TokenType>(i));
    for (int i = 0; i < LOCATION; i++)
        setToken(s, static_cast<e_TokenType>(i));
    std::cout << ErrorPages(s.error_pages).content(404);
    _errorPages = ErrorPages(l.data.error_pages);
    if (!_errorPages.getNbPages())
        _errorPages = ErrorPages(s.error_pages);
    std::cout << YELLOW << _errorPages.content(404) << RESET;
}

RequestServer::RequestServer(const RequestServer &serv)
{
    *this = serv;
}

RequestServer::~RequestServer() { }

/*  Ne me jugez pas j'avais la flemme ... */
void    RequestServer::setToken(t_ServerData serv, e_TokenType type)
{
    if (!undefined(type))
        return;
    switch (type)
    {
        case HOST:
            _host = serv.host;
            break;
        case LISTEN:
            _port = serv.port;
            break;
        case SERVER_NAME:
            _serverName = serv.server_name;
            break;
        case ROOT_PATH:
            _root = serv.root;
            break ;
        case HTLM_INDEX:
            _index = serv.index;
            break;
        case AUTOINDEX:
            _autoindex = serv.autoindex;
            break;
        case ERROR_PAGE:
            break ;
       //     _errorPages = serv.errorPages();
            break;
        case UPLOAD_STORAGE:
            _storage = serv.upload_storage;
            break;
        case CGI_DATA:
            _cgi = serv.cgi;
            break;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = serv.client_max_body_size;
            break;
        case METHODS:
            _methods = serv.methods;
            break;
        case REDIRECT:
            _redirect = serv.redirect;
            break ;
        default:
            break;
    }
}

void    RequestServer::setToken(t_Location loc, e_TokenType type)
{
    //if (Location(loc).undefined(type))
    //    return ;
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
            _root = loc.data.root;
            break ;
        case HTLM_INDEX:
            _index = loc.data.index;
            break;
        case AUTOINDEX:
            _autoindex = loc.data.autoindex;
            break;
        case ERROR_PAGE:
        //    _errorPages = loc.errorPages(); // check if push_back instead
            break ;
        case UPLOAD_STORAGE:
            _storage = loc.data.upload_storage;
            break;
        case CGI_DATA:
            _cgi = loc.data.cgi;
            break ;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = loc.data.client_max_body_size;
            break;
        case METHODS:
            _methods= loc.data.methods;
            break;
        case REDIRECT:
            _redirect = loc.data.redirect;
            break ;
        default:
            break;
    }
}

bool    RequestServer::has(e_TokenType type)
{
    return (!undefined(type));
}

bool    RequestServer::undefined(e_TokenType type)
{
    Color   c;
    std::string u = "UNDEFINED";

    switch (type)
    {
        case HOST:
            if (_host.empty() || _host == u || _host == c.red() + u + c.reset())
                return true; 
            return false;
        case LISTEN:
            if (_port.empty() || _port == u || _port == c.red() + u + c.reset())
                return true;
            return false;
            break;
        case SERVER_NAME:
            if (_serverName.empty() || _serverName == u || _serverName == c.red() + u + c.reset())
                return true;
            return false;
        case ROOT_PATH:
            if (_root.empty() || _root == u || _root == c.red() + u + c.reset())
                return true;
            return false;
        case HTLM_INDEX:
            if (_index.empty() || _index == u || _root == c.red() + u + c.reset())
                return true;
            return false;
        case AUTOINDEX:
            return _autoindex;
        case ERROR_PAGE:
            if (!_errorPages.getNbPages())
                return true;
            return false;
        case UPLOAD_STORAGE:
            if (_storage.empty() || _storage == u || _storage == c.red() + u + c.reset())
                return true;
            return false;
        case CGI_DATA:
            if (_cgi.empty())
                return true;
            return false;
        case CLIENT_MAX_BODY_SIZE:
            if (_clientBodySize <= 1)
                return true;
            return false;
        case METHODS:
            if (_methods.empty())
                return true;
            return false;
        case REDIRECT:
            if (_redirect.empty() || _redirect == "UNDEFINED" || _redirect == c.red() + u + c.reset())
                return true;
            return false;
        default:
            break;
    }
    return (true);
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
    stream << "Nb error pages       : " << rs.errorPages().getNbPages();
    
  //  for (int j = 0; j < OTHER; j++)
        //stream <<  rs.errorPages().content(static_cast<RequestError>(j));
         //  stream << "\t\t" << j->first << " <==> " << j->second << '\n';
    stream << '\n';
    return (stream);
}