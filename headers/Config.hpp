#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Error.hpp"
# include "Debug.hpp"

# define BEGIN 0
# define END 1
//
        //"server_name",
        //"root",
        //"index",
        //"autoindex",
        //"error_page",
        //"upload_storage",
        //"cgi_ext",
        //"cgi_path",
        //"client_max_body",
        //"location",
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
    CGI_EXTENTION,
    CGI_PATH,
    CLIENT_MAX_BODY_SIZE,  // max request body
    METHODS,
    LOCATION,              // start of a location block
    TOKEN_TYPE_COUNT       //                       7
}   e_TokenType;

//typedef enum LocationTokens
//{
//    METHODS,                // accepted HTTP methods    0
//    HTTP_REDIRECTION,       // redirection URL or code
//    FILE_PATH,              // local file path
//    DIR_LISTING,            // autoindex on/off
//    DEFAULT_FILE,           // default file for directories
//    UPLOAD_STORAGE,         // where uploads are saved
//    CGI_EXTENSION,          // CGI extensio
//    CGI_PATH,               // path to CGI executable
//    LOCATION_TOKEN_COUNT    //                          9
//}   e_LocationToken;

# define NUM_MAIN_TOKENS TOKEN_TYPE_COUNT
//# define NUM_LOC_TOKENS LOCATION_TOKEN_COUNT
typedef struct s_ServerData t_ServerData;
typedef struct s_Location   t_Location;

struct s_ServerData
{
    std::map<int, std::string>  error_pages;        // code -> file path
    std::vector<t_Location>     locations;
    std::vector<std::string>    methods;
    std::string                 host;
    std::string                 port;
    std::string                 server_name;
    std::string                 root;
    std::string                 index;
    std::string                 upload_storage;
    std::string                 cgi_ext;
    std::string                 cgi_path;
    size_t                      client_max_body_size;
    bool                        autoindex;
    bool                        isLoc;
};

struct s_Location
{
   bool            active;
   t_ServerData    data;
   std::string     path;
};

class Config
{
    Debug                                   *_dfile;
    int                                     _nbServers;
    std::string                             _content;
    std::string                             _Tokens[NUM_MAIN_TOKENS + 1];
    std::vector<t_ServerData>               _servers;
    void                                    initTokenMaps();
    public:
        Config(std::string fileName, Debug &dfile);
        Config(const Config &);
        ~Config();


        Config  &operator=(const Config &);

        //  setters
        void            setServerData(t_ServerData data);

        //  getters
//        void            *getServerParam(int serverID, std::string param) const;
        t_ServerData    getServerData(int serverID) const;
        int             getNbServers() const;
        void            printServers() const;
    
        //  active parsing
        bool            checkServerData(int index) const;
        void            parseLocation(t_ServerData &serv, std::string &content, std::string getTokenLine);
        void            parseContent();

        //  UTILS
        void            assignToken(t_Location &loc, std::string &content, size_t pos, int type);
        void            assignToken(t_ServerData &serv, std::string &content, size_t pos, int type);
        size_t          findToken(std::string content, size_t range[2], e_TokenType i);

        //  generic errors
        class BadFileException : public std::exception
        {
            public:
                const char  *what() const throw();
        };
        class MissingParamException : public std::exception
        {
            public:
                const char  *what() const throw();
        };
        class BadParamException : public std::exception
        {
            public:
                const char  *what() const throw();
        };
        class ParseErrorExemption : public std::exception
        {
            public:
                const char  *what() const throw();
        };
        class OutOfBoundsExeption : public std::exception
        {
            public:
                const char *what() const throw();
        };
};

std::ostream    &operator<<(std::ostream &stream, Config &conf);

void    reset(t_ServerData &serv, std::string &content, size_t &pos, size_t &rBegin, size_t &rEnd);
void    reset(t_Location &loc, std::string &content, size_t &pos, size_t &rBegin, size_t &rEnd);
void    sanitizeLine(std::string &line);
std::string getTokenLine(const std::string &content, const std::string &token, size_t pos);
void eraseLine(std::string &content, const std::string &line);
size_t  findNextSpace(std::string line, size_t &from);
size_t  getNb(std::string line, std::string token);
std::string getStr(std::string &line, std::string token);
void    assignDefaultToken(t_ServerData &serv, std::string &content, size_t pos, int type);
t_ServerData    getDefaultServ(bool with_location);

#endif
