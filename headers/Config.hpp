#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Error.hpp"
# include "Debug.hpp"

# define BEGIN 0
# define END 1

typedef enum TokenTypes
{
    HOST,
    LISTEN,              // interface:port        0
    SERVER_NAME,
    ROOT_PATH,           // virtual host name
    HTLM_INDEX,
    AUTOINDEX,
    ERROR_PAGE,            // default error pages
    UPLOAD_STORAGE,
    CGI_DATA,
    CLIENT_MAX_BODY_SIZE,  // max request body
    METHODS,
    REDIRECT,
    LOCATION,              // start of a location block
    LOCATION_PATH,
    TOKEN_TYPE_COUNT       //                       7
}   e_TokenType;

# define NUM_MAIN_TOKENS TOKEN_TYPE_COUNT
//# define NUM_LOC_TOKENS LOCATION_TOKEN_COUNT
typedef struct s_ServerData t_ServerData;
typedef struct s_Location   t_Location;

struct s_ServerData
{
//    std::vector<std::map<int, std::string>> pages;  // new
    std::map<int, std::string>              error_pages;        // code -> file path
    std::map<std::string, std::string>      cgi;
    std::vector<t_Location>                 locations;
    std::vector<std::string>                methods;
    std::string                             host;
    std::string                             port;
    std::string                             server_name;
    std::string                             root;
    std::string                             index;
    std::string                             upload_storage;
    std::string                             redirect;
    size_t                                  client_max_body_size;
    bool                                    autoindex;
    bool                                    isLoc;
};

struct s_Location
{
   bool            active;
   t_ServerData    data;
   std::string     path;
};

class Config
{

    protected:
        Debug                                   *_dfile;
        int                                     _nbServers;
        std::string                             _fileName;
        std::string                             _content;
        std::string                             _Tokens[NUM_MAIN_TOKENS + 1];
        std::vector<t_ServerData>               _servers;
    private:
        void                                    initTokenMaps();
    public:
        Config();
        Config(std::string fileName, Debug &dfile);
        Config(const Config &);
        virtual ~Config();

        Config  &operator=(const Config &);

        //  setters
        void            setServerData(t_ServerData data);

        //  getters
//        void            *getServerParam(int serverID, std::string param) const;
        t_ServerData    getServerData(int serverID) const;
        int             getNbServers() const;

        //  nice getters
        size_t          find(std::string token, e_TokenType type);
        
        //  active parsing
        bool            checkServerData(int index) const;
        void            parseLocation(t_ServerData &serv, std::string &content, std::string getTokenLine);
        void            parseContent();

        //  UTILS
        void            assignToken(t_Location &loc, std::string &content, size_t pos, int type);
        void            assignToken(t_ServerData &serv, std::string &content, size_t pos, int type);
        size_t          findToken(std::string &content, size_t range[2], e_TokenType i);
        void            sanitize();
};

std::ostream    &operator<<(std::ostream &stream, Config &conf);

void            sanitizeLine(std::string &line);
std::string     getTokenLine(const std::string &content, const std::string &token, size_t pos);
void            eraseLine(std::string &content, const std::string &line);
size_t          findNextSpace(std::string line, size_t &from);
size_t          getNb(std::string line, std::string token);
std::string     getStr(std::string &line, std::string token);
void            assignDefaultToken(t_ServerData &serv, std::string &content, size_t pos, int type);
t_ServerData    getDefaultServ(bool with_location);

#endif
