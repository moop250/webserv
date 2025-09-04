# include "Server.hpp"

Location::Location() : _path("UNDEFINED"),
    _root("UNDEFINED"), _index("UNDEFINED"), _upload_storage("UNDEFINED"),
    _redirect("UNDEFINED"), _client_max_size(1), _autoindex(false)
{
    _cgi.insert(std::make_pair("UNDEFINED", "UNDEFINED"));
    _errorPages = ErrorPages();
// _error_pages.insert(std::make_pair(0, "UNDEFINED"));
    _methods.push_back("UNDEFINED");
    return ;
}

Location::Location(t_Location loc)
{
    _path = loc.path;
    _cgi = loc.data.cgi;
    _errorPages = ErrorPages(loc.data.error_pages);
    for (std::map<int, std::string>::iterator i = loc.data.error_pages.begin(); i != loc.data.error_pages.end(); i++)
        std::cout << i->first;
//    _error_pages = loc.data.error_pages;
    _methods = loc.data.methods;
    _root = loc.data.root;
    _index = loc.data.index;
    _upload_storage = loc.data.upload_storage;
    _client_max_size = loc.data.client_max_body_size;
    _autoindex = loc.data.autoindex;
    foo = loc.path;
    return ;
}

Location::Location(t_ServerData serv)
{
    _path = "UNDEFINED";
    _errorPages = ErrorPages(serv.error_pages);

    _cgi = serv.cgi;
    //for (std::map<std::string, std::string>::iterator i = serv.cgi.begin(); i != serv.cgi.end();i++)
    //{
    // //   std::cout << i->first << " and " << i->second << std::endl;
    //    _cgi.insert(std::make_pair(i->first, i->second));
    //}
    for (std::vector<std::string>::iterator i = serv.methods.begin(); i != serv.methods.end(); i++)
    {
     //   std::cout << *i;
        _methods.push_back(*i);
    }
    _root = serv.root;
    _index = serv.index;
    _upload_storage = serv.upload_storage;
    _client_max_size = serv.client_max_body_size;
    _autoindex = serv.autoindex;
    _redirect = serv.redirect;
}

Location::Location(const Location &l) :
    _path(l.path()), _root(l.root()),
    _index(l.index()), _upload_storage(l.storage()), _redirect(l.redirect()),
    _client_max_size(l.clientSize()), _autoindex(l.autoindex())
{
    //  cp cgi
    //  cp errorpages
    //  cp methods
    return ;
}

Location::~Location() {}

Location    &Location::operator=(const Location &l)
{
    if (this != &l)
        *this = l;
    return *this;
}

bool    Location::undefined(e_TokenType type)
{
    int general = TOKEN_TYPE_COUNT;
    Color      c;

    switch (type)
    {
        case TOKEN_TYPE_COUNT:
            while (general-- > 0)
            {
                if (undefined(static_cast<e_TokenType>(general)))
                    continue;
                else
                    return (false);
            }
            return (true);
        case ROOT_PATH:
            if (_root == "UNDEFINED" || _root == c.red() + "UNDEFINED" + c.reset() || _root.empty())
                return true;
            return (false);
        case HTLM_INDEX:
            if (_index == "UNDEFINED" || _index == c.red() + "UNDEFINED" + c.reset() || _index.empty())
                return true;
            return (false);
        case AUTOINDEX:
            if (_autoindex == false)
                return true;
            return (false);
        case ERROR_PAGE:
            //  if empty
            return (false);
        case UPLOAD_STORAGE:
            if (_upload_storage == "UNDEFINED" || _upload_storage == c.red() + "UNDEFINED" + c.reset() || _upload_storage.empty())
                return true;
            return (false);
        case CGI_DATA:
            if (_cgi.empty())
                return true;
            return (false);
        case METHODS:
            if (_methods.empty())
                return true;
            return (false);
        case REDIRECT:
            if (_redirect == "UNDEFINED" || _redirect == c.red() + "UNDEFINED" + c.reset() || _redirect.empty())
                return true;
            return (false);
        case CLIENT_MAX_BODY_SIZE:
            if (_client_max_size)
                return (true);
            return (false);
      //  case LOCATION_PATH:
      //      if ( == _path)
      //          return true;
      //      return (false);
        default:
            break;
    }
    return true;
}

bool    Location::has(std::string token, e_TokenType type = TOKEN_TYPE_COUNT)
{
    int general = TOKEN_TYPE_COUNT;

    switch (type)
    {
        case TOKEN_TYPE_COUNT:
            while (general-- > 0)
                if (has(token, static_cast<e_TokenType>(general)))
                    return true;
            return (false);
        case ROOT_PATH:
            if (token == _root)
                return true;
            return (false);
        case HTLM_INDEX:
            if (token == _index)
                return true;
            return (false);
        case AUTOINDEX:
            if (token == "ON" || "on")
                return true;
            return (false);
        case ERROR_PAGE:
            if (_errorPages.find(static_cast<RequestError>(atoi(token.c_str()))))
                return true;
            return (false);
        case UPLOAD_STORAGE:
            if (token == _upload_storage)
                return true;
            return (false);
        case CGI_DATA:
            if (token == _cgi.find(token)->second || token == _cgi.find(token)->first)
                return true;
            return (false);
        case METHODS:
            for (std::vector<std::string>::iterator i = _methods.begin(); i != _methods.end(); i++)
                if (token == *i)
                    return true;
            return (false);
        case LOCATION_PATH:
            if (token == _path)
                return true;
            return (false);
        default:
            break;
    }
    return false;
}

template <typename Token>
Token   Location::attributl(e_TokenType type)
{
    switch (type)
    {
        case ROOT_PATH:
            return _root;
        case HTLM_INDEX:
            return (_index);
        case AUTOINDEX:
            return (_autoindex);
        case ERROR_PAGE:
            return (_errorPages);
        case UPLOAD_STORAGE:
            return (_upload_storage);
        case CGI_DATA:
            return (_cgi);
        case METHODS:
            return (_methods);
        case LOCATION_PATH:
            return (_path);
        default:
            break;
    }
    return ;
}

std::string Location::path() const { return _path; }

ErrorPages  Location::errorPages() const { return _errorPages; }

std::map<std::string, std::string> Location::cgi() const { return _cgi; }

std::vector<std::string>    Location::methods() const { return _methods; }

std::string Location::root() const { return _root; }

std::string Location::index() const { return _index; }

std::string Location::storage() const { return _upload_storage; }

size_t  Location::clientSize() const { return _client_max_size; }

bool    Location::autoindex() const { return _autoindex; }

std::string Location::redirect() const { return _redirect; }

std::ostream &operator<<(std::ostream &stream, const Location &loc)
{
    stream << "Loc path : " << loc.path() << std::endl;
    stream << ROSE << "LOCATION CONTENT\n";
    stream << "server name : " << " no nome" << '\n';
    stream << "client size : " << loc.clientSize() << '\n'
        << "Redirection : " << loc.redirect() << '\n'
        << "Storage : " << loc.storage() << '\n'
        << "methods : ";
    for (size_t i = 0; i < loc.methods().size(); i++)
        stream << " " << loc.methods().at(i);
    stream << "\nCgi : \n";

    return stream;
}
