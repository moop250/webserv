#include "RequestServer.hpp"

RequestServer::RequestServer() {
    std::cerr << RED << "Do not use a request server alone \n" << RESET;
}

RequestServer::RequestServer(Config config, std::string servName, std::string locPath)
{
    (void)config;
    (void)servName;
        (void)locPath;
    return ;
}

RequestServer::RequestServer(const RequestServer &serv)
{
    (void)serv;
    return ;
}

RequestServer::~RequestServer() {}

RequestServer   &RequestServer::operator=(const RequestServer &srv)
{
    (void)srv;
    return *this;
}