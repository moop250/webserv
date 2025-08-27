# include "Server.hpp"

Server::Server() : Location(),
    _host("UNDEFINED"), _port("UNDEFINED"), _server_name("UNDEFINED")
{
    return ;
}

Server::Server(t_ServerData serv) : Location(serv)
{
    //  handle locations
    _host = serv.host;
    _port = serv.port;
    _server_name = serv.server_name; 
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

Server::~Server() {
}

Server    &Server::operator=(const Server &s)
{
    if (this != &s)
        *this = s;
    return *this;
}

bool    Server::has(e_TokenType type)
{
    (void)type;
    return true;
}

std::string Server::host() const { return _host; }

std::string Server::port() const { return _port; }

std::string Server::name() const { return _server_name; }

Location    Server::location(int at) const {
    try {
        return _locations.at(at);
    } catch(...) {
        throw std::out_of_range("No location at this range in server");
    }
}

std::vector<Location>   Server::locations() const { return _locations; }
