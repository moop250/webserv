# include "Server.hpp"

template <typename Token>
Server<Token>::Server() : Location(),
    _host("UNDEFINED"), _port("UNDEFINED"), _server_name("UNDEFINED"),
    _root("UNDEFINED"), _index("UNDEFINED"), _upload_storage("UNDEFINED"),
    _client_max_size(1), _autoindex(false)
{
    //  init cgi
    //  init errorpages
    //  init location
    //  init methods
    return ;
}

template <typename Token>
Server<Token>::Server(t_ServerData serv) : Location(serv)
{
    //  update t_servData in config before coding this
    return ;
}

template <typename Token>
Server<Token>::Server(const Server &s) : Location(),
    _host(s.attribut("host")), _port(s.attribut("port")),
    _server_name(s.attribut("server_name")), _root(s.attribut("root")),
    _index(s.attribut("index")), _upload_storage(s.attribut("upload_storage")),
    _client_max_size(s.attribut("client_max_size")), _autoindex(s.attribut("autoindex"))
{
    //  cp cgi
    //  cp errorpages
    //  cp location
    //  cp methods
    return ;
}

template <typename Token>
Server<Token>::~Server() {}

template <typename Token>
Server<Token>    &Server<Token>::operator=(const Server<Token> &s)
{
    if (this != &s)
        *this = s;
    return *this;
}

template <typename Token>
bool    Server<Token>::has(Token token)
{
    return true;
}

template <typename Token>
Token    Server<Token>::attribut(std::string tokenType, int member)
{
    Token   tok;

    return tok;
}

//template <typename Token>
//Server<Token>::
//
//template <typename Token>
//Server<Token>::
//
//template <typename Token>
//Server<Token>::
//
//template <typename Token>
//Server<Token>::
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