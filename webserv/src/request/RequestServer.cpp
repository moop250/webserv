#include "RequestServer.hpp"

RequestServer::RequestServer(bool def) {
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
    _location = "";
    if (!def)
        return ;
    std::cout << CYAN << "Default server configuration\n" << RESET << std::flush;
    _isValid = true;
    _serverName = "Aaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    _host = "127.0.0.1";
    _port = "8001";
    _root = "ressources/";
    _index = "index.html";
    _storage = "upload";
    _redirect = "301 redirection.html";
    _autoindex = true;
    _clientBodySize = 3000000;
    _methods.push_back("GET");
    _methods.push_back("POST");
    _methods.push_back("DELETE");
    _errorPages = ErrorPages();
    _cgi.insert(std::make_pair("say_hello.py", ".py"));
}

RequestServer::RequestServer(Config config)
{
    t_ServerData    s = config.getServerData(0);

    for (int i = 0; i < LOCATION; i++)
        setToken(s, static_cast<e_TokenType>(i));
    _isValid = 1;
    _isLocation = 0;
    _location = "";
}

static size_t   findServer(Config config, std::string ip, std::string port)
{
    for (int i = 0; i < config.getNbServers() ; i++)
    {
        const t_ServerData serv = config.getServerData(i);
        if ((serv.host == ip || serv.host == "0.0.0.0") && serv.port == port)
            return i;
    }
    std::cout << YELLOW << "server host:port not found\n" << RESET;
    return std::string::npos;
}

static size_t   findServer(Config config, std::string name)
{
    for (int i = 0; i < config.getNbServers(); i++)
    {
        const t_ServerData serv = config.getServerData(i);
        if (serv.server_name == name)
            return i;
    }
    std::cout <<  YELLOW <<"server name not found\n" << RESET;
    return std::string::npos;
}

/* TRIER QUELQUES CAS (CERTAINS ME SEMBLENT SUSPECTS)*/
static size_t   findLocation(std::string path, t_ServerData server)
{
    // int     iteration = 0;
    size_t  id = 0;
    std::string root = server.root;

//    std::cout << "PATH TO FIND IN LOCATION : " << path << std::endl;
    if (root.find("UNDEFINED") != std::string::npos)
        root = "";
    for (std::vector<t_Location>::iterator i = server.locations.begin(); i < server.locations.end(); i++)
    {
        // std::cout << "COMPARING WITH PATH IN SERVER : " << iteration++ << " : " << i->path << std::endl;
        if (path.find(i->path) != std::string::npos
            || levenshtein(i->path, root + path) < 2
            || path == i->path
            || levenshtein(i->path, path) < 2
        )
            return id;
        id++;
    }
    std::cout <<  YELLOW << "\nPath doesn't match any location\n" << RESET;
    return std::string::npos;
}

RequestServer::RequestServer(Config config, std::string port, std::string ip, std::string name, std::string path) :
    _isLocation(0), _location("")
{
    size_t  servId;
    size_t  locId;

    if ((servId = findServer(config, ip, port)) == std::string::npos)
        if ((servId = findServer(config, name)))
            return ;
    
    t_ServerData    server = config.getServerData(servId);
    if ((locId = findLocation(path, server)) != std::string::npos)
    {
        t_Location  location = server.locations.at(locId);
        for (int i = 0; i < LOCATION; i++)
            setToken(location, static_cast<e_TokenType>(i));
        _location = location.path;
    }

    for (int i = 0; i < LOCATION; i++)
        setToken(server, static_cast<e_TokenType>(i));
    _isValid = true;
    //  find ip:port
    //  find server_name
    //  find path
}

RequestServer::RequestServer(const RequestServer &serv)
{
    *this = serv;
}

RequestServer::~RequestServer() { }

void    RequestServer::setToken(t_ServerData serv, e_TokenType type)
{
    if (this->has(type) && type != CGI_DATA)
    {
        return ;
    }
    switch (type)
    {
        case HOST:
            _host = serv.host;
            break ;
        case LISTEN:
            _port = serv.port;
            break ;
        case SERVER_NAME:
            _serverName = serv.server_name;
            break ;
        case ROOT_PATH:
            _root = serv.root;
            break ;
        case HTLM_INDEX:
            _index = serv.index;
            break ;
        case AUTOINDEX:
            _autoindex = serv.autoindex;
            break ;
        case ERROR_PAGE:
            //      todo --> sub to default
            for (std::map<int, std::string>::iterator i = serv.error_pages.begin(); i != serv.error_pages.end(); i++)
            {
                if (_errorPages.has(i->first, i->second))
                    _errorPages.set(i->first, i->second)
            _errorPages = ErrorPages(serv.error_pages);
            break ;
        case UPLOAD_STORAGE:
            _storage = serv.upload_storage;
            break ;
        case CGI_DATA:
            for (std::map<std::string, std::string>::iterator i = serv.cgi.begin(); i != serv.cgi.end(); i++)
                _cgi.insert(std::make_pair(i->first, i->second));
            break ;
        case CLIENT_MAX_BODY_SIZE:
            _clientBodySize = serv.client_max_body_size;
            break ;
        case METHODS:
            _methods = serv.methods;
            break ;
        case REDIRECT:
            _redirect = serv.redirect;
            break ;
        default:
            break ;
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
            _errorPages = ErrorPages(loc.data.error_pages);
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
            if (_clientBodySize <= 10)
                return true;
            return true;
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
        _location = srv._location;
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

RequestServer   RequestServer::defaultServ() const {
    return RequestServer(1);
}

std::string                         RequestServer::getLocation() const {
    return _location;
}

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
