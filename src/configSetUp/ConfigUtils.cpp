#include "Config.hpp"

t_ServerData    getDefaultServ(bool with_location)
{
    Color   c;

    const t_ServerData  default_server_values = {
        .server_name = c.red() + "UNDEFINED" + c.reset(),
        .root = c.red() + "UNDEFINED" + c.reset(),
        .index = c.red() + "UNDEFINED" + c.reset(),
        .upload_storage = c.red() + "UNDEFINED" + c.reset(),
        .cgi_ext = c.red() + "UNDEFINED" + c.reset(),
        .cgi_path = c.red() + "UNDEFINED" + c.reset(),
        .client_max_body_size = 1,
        .autoindex = false
    };

    const t_Location    default_location_values = {
        .active = false,
        .data = default_server_values
    };

    t_ServerData    defaultServ = default_server_values;

    defaultServ.locations.push_back(default_location_values);
    if (!with_location)
        return defaultServ;
    return (defaultServ);
}

void    assignDefaultToken(t_ServerData &serv, std::string &content, size_t pos, int type)
{
    (void)content;
    (void)pos;
     switch (type)
    {
        case HOST:
            serv.host = "UNDEFINED";
                break ;
        case LISTEN:
            serv.port ="UNDEFINED";
            break ;
        case SERVER_NAME:
            serv.server_name = "UNDEFINED";
            break ;
        case ROOT_PATH:
            serv.root = "UNDEFINED";
            break ;
        case HTLM_INDEX:
            serv.index = "UNDEFINED";
            break ;
        case AUTOINDEX:
            serv.autoindex = false;
            break ;
        case ERROR_PAGE:
            serv.error_pages.insert(std::make_pair(404, "UNDEFINED"));
            break ;
        case UPLOAD_STORAGE:
            serv.upload_storage = "UNDEFINED";
            break ;
        case CGI_EXTENTION:
            serv.cgi_ext = "UNDEFINED";
            break ;
        case CGI_PATH:
            serv.cgi_path = "UNDEFINED";
            break ;
        case CLIENT_MAX_BODY_SIZE:
            serv.client_max_body_size = 0;
            break ;
        case METHODS:
            serv.methods.push_back("NO METHOD");
            break ;
        case LOCATION:
            serv.locations.push_back(getDefaultServ(1).locations.at(0));
            break ;
        //    parseLocation(serv, content);
          //  break ;
        default:
            break ;
    }
}

std::ostream    &operator<<(std::ostream &stream, Config &conf)
{
    t_ServerData    print;
    int iteration = 0;

    for (int i = 0; i < conf.getNbServers(); i++)
    {
        stream << "Server n* " << i << '\n';
        print = conf.getServerData(i);
        stream  << "\tServer name    : " << print.server_name << '\n'
                << "\tRoot path      : " << print.root << '\n'
                << "\tIndex file     : " << print.index << '\n'
                << "\tAuto index     : " << (print.autoindex ? "On" : "OFF") << '\n'
                << "\tUpload storage : " << print.upload_storage << '\n'
                << "\tCgi external   : " << print.cgi_ext << '\n'
                << "\tCgi path       : " << print.cgi_path << '\n'
                << "\tMax client siz : " << print.client_max_body_size << std::endl;
        stream << "\tMethods        : ";
        for (std::vector<std::string>::iterator i = print.methods.begin(); i != print.methods.end(); i++)
            stream << " " << *i;
        stream << '\n';
        stream << "\tHost           : ";
        stream << print.host;
        stream << '\n' << "\tListeners      : ";
        stream << print.port;
        stream << "\n";
        stream << "\terror pages    : \n";
        if (print.error_pages.empty())
            stream << RED << "\t\tUNDEFINED" << RESET;
        else 
            for (std::map<int, std::string>::iterator j = print.error_pages.begin(); j != print.error_pages.end(); j++)
                stream << "\t\tnb : " << j->first << " second : " << j->second << '\n';
        stream << '\n';
        iteration = 0;
        for (std::vector<t_Location>::iterator i = print.locations.begin(); i != print.locations.end(); i++)
        {
            stream << "\tLocations : " << iteration++  << '\n'
                << "\t\tLocation path           : " << i->path << '\n'
                << "\t\tRoot path               : " << i->data.root << '\n'
                << "\t\tAuto index status       : " << (i->data.autoindex ? "On" : "OFF") << '\n'
                << "\t\tHTLM index file         : " << i->data.index << '\n'
                << "\t\tUpload storage          : " << i->data.upload_storage << '\n'
                << "\t\tCgi external entity     : " << i->data.cgi_ext << '\n'
                << "\t\tCgi path to interpreter : " << i->data.cgi_path << '\n'
                << "\t\tMethods allowed         : ";
            if (i->data.methods.empty())
                stream << RED << "UNDEFINED" << RESET;
            else
                stream << GREEN << "PRINTING METHODS" << RESET;
        //    std::cerr << "hello";
        //    if (i->data.methods.empty())
        //        continue ;
        //    for (std::vector<std::string>::iterator j = i->data.methods.begin(); j != i->data.methods.end(); ++i)
        //        stream << *j << ' ';
            stream << "\n\n";
        }
        stream << "\n\n";
    }
    return (stream);
}
