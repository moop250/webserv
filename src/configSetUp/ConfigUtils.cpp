#include "Config.hpp"

std::ostream    &operator<<(std::ostream &stream, Config &conf)
{
    t_ServerData    print;

    for (int i = 0; i < conf.getNbServers(); i++)
    {
        stream << "Server n* " << i << '\n';
        print = conf.getServerData(i);
        stream // << "\tHost name      : " << print.hosts.at(0) << '\n'
                << "\tServer name    : " << print.server_name << '\n'
                << "\tRoot path      : " << print.root << '\n'
                << "\tIndex file     : " << print.index << '\n'
                << "\tAuto index     : " << (print.autoindex ? "On" : "OFF") << '\n'
                << "\tUpload storage : " << print.upload_storage << '\n'
                << "\tCgi external   : " << print.cgi_ext << '\n'
                << "\tCgi path       : " << print.cgi_path << '\n'
                << "\tMax client siz : " << print.client_max_body_size << std::endl;
        stream << "\tMethods : ";
        for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); i++)
            stream << " " << *i;
        stream << '\n';
        stream << "\tHost : ";
        stream << print.host;
        stream << '\n' << "\tListeners (port) : ";
        stream << print.port;
        stream << "\n";
        for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); i++)
        {
            static int iteration = 0;
            stream << "\tLocations :" << iteration++  << '\n'
                << "\t\tLocation path           :" << i->path << '\n'
                << "\t\tRoot path               : " << i->data.root << '\n'
                << "\t\tAuto index status       : " << (i->data.autoindex ? "On" : "OFF") << '\n'
                << "\t\tHTLM index file         : " << i->data.index << '\n'
                << "\t\tUpload storage          : " << i->data.upload_storage << '\n'
                << "\t\tCgi external entity     : " << i->data.cgi_ext << '\n'
                << "\t\tCgi path to interpreter : " << i->data.cgi_path << '\n'
                << "\t\tMethods allowed : ";
            for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); ++i)
                stream << *i << ' ';
            stream << "\n\n";
        }
        stream << "\n\n";
    }
    return (stream);
}
