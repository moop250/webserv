# include "Server.hpp"

template <typename Token>
Location<Token>::Location() : _path("UNDEFINED")
    _root("UNDEFINED"), _index("UNDEFINED"), _upload_storage("UNDEFINED"),
    _client_max_size(1), _autoindex(false)
{
    _cgi.insert(std::make_pair("UNDEFINED", "UNDEFINED"));
    _error_pages.insert(std::make_pair("000", "UNDEFINED"));
    _methods.push_back("UNDEFINED");
    return ;
}

template <typename Token>
Location<Token>::Location(t_Location loc)
{
    _path = loc.path;
    _cgi = loc.data.cgi;
    _error_pages = loc.data.error_pages;
    _methods = loc.data.methods;
    _root = loc.data.root;
    _index = loc.data.index;
    _upload_storage = loc.data.upload_storage;
    _client_max_size = loc.data.client_max_body_size;
    _autoindex = loc.data.autoindex;
    return ;
}

template <typename Token>
Location<Token>::Location(const Location &l) :
    _path(l.attribut("path")), _root(l.attribut("root")),
    _index(l.attribut("index")), _upload_storage(l.attribut("upload_storage")),
    _client_max_size(l.attribut("client_max_size")), _autoindex(l.attribut("autoindex"))
{
    //  cp cgi
    //  cp errorpages
    //  cp methods
    return ;
}

template <typename Token>
Location<Token>::~Location() {}

template <typename Token>
Location<Token>    &Location<Token>::operator=(const Location<Token> &l)
{
    if (this != &l)
        *this = l;
    return *this;
}

template <typename Token>
bool    Location<Token>::has(Token token)
{
    return true;
}

template <typename Token>
Token    Location<Token>::attribut(std::string tokenType, int member)
{
    Token   tok;

    return tok;
}


//template <typename Token>
//Location<Token>::
//
//template <typename Token>
//Location<Token>::
//
//template <typename Token>
//Location<Token>::
//
//template <typename Token>
//Location<Token>::
//
//template <typename Token>
//
//
//template <typename Token>
//
//
//template <typename Token>
//
//
//template <typename Token>
//
//