# include "Server.hpp"

Location::Location() : _path("UNDEFINED"),
    _root("UNDEFINED"), _index("UNDEFINED"), _upload_storage("UNDEFINED"),
    _client_max_size(1), _autoindex(false)
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
    _cgi = serv.cgi;
    _errorPages = ErrorPages(serv.error_pages);
    for (std::map<int, std::string>::iterator i = serv.error_pages.begin(); i != serv.error_pages.end(); i++)
        std::cout << i->first << "\n";

//    _error_pages = serv.error_pages;
    _methods = serv.methods;
    _root = serv.root;
    _index = serv.index;
    _upload_storage = serv.upload_storage;
    _client_max_size = serv.client_max_body_size;
    _autoindex = serv.autoindex;
}

Location::Location(const Location &l) :
    _path(l.path()), _root(l.root()),
    _index(l.index()), _upload_storage(l.storage()),
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

bool    Location::has(e_TokenType type)
{
    (void)type;
    return true;
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


std::ostream &operator<<(std::ostream &stream, const Location &loc)
{
    stream << "Loc path : " << loc.path() << std::endl;
    return stream;
}
