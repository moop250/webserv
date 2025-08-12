#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Error.hpp"
# include "Debug.hpp"
# include <vector>
# include <map>

typedef enum TokenTypes
{
    LISTENER,              // interface:port        0
    SERVER_NAME,           // virtual host name
    ERROR_PAGE,            // default error pages
    CLIENT_MAX_BODY_SIZE,  // max request body
    ROOT_PATH,             // root directory for server
    LOCATION,              // start of a location block
    ALL,
    TOKEN_TYPE_COUNT       //                       7
} e_TokenType;

typedef enum LocationTokens
{
    METHODS,                // accepted HTTP methods    0
    HTTP_REDIRECTION,       // redirection URL or code
    FILE_PATH,              // local file path
    DIR_LISTING,            // autoindex on/off
    DEFAULT_FILE,           // default file for directories
    UPLOAD_STORAGE,         // where uploads are saved
    CGI_EXTENSION,          // CGI extensio
    CGI_PATH,               // path to CGI executable
    LOC_ALL,
    LOCATION_TOKEN_COUNT    //                          9
} e_LocationToken;

# define NUM_MAIN_TOKENS TOKEN_TYPE_COUNT
# define NUM_LOC_TOKENS LOCATION_TOKEN_COUNT


typedef struct Location
{
    std::string                 loc;            // path of location
    std::string                 methods[5];        // GET, POST, etc.
    std::string                 redirection;    // could be "301 /newpath"
    std::string                 root;           // path mapping for this location
    bool                        autoIndex;      // true/false
    std::string                 defaultFile;    // index.html
    std::string                 uploadDir;      // upload storage
    std::string                 cgi_ext;        // .php, .py
    std::string                 cgi_path;       // path to interpreter
}   t_Location;

typedef struct ServerData
{
    std::vector<std::string>    listeners;          // multiple host:port
    std::string                 server_name;
    std::map<int, std::string>  error_pages;        // code -> file path
    size_t                      client_max_body_size;
    std::string                 root;
    std::vector<t_Location>     locations;
}   t_ServerData;

extern const t_Location    default_location_values;
extern const t_ServerData  default_server_values;

class Config
{
    Debug                                   *_dfile;
    std::vector<t_ServerData>               _servers;
    std::string                             _content;
    std::map<std::string, e_LocationToken>  _locTokenMap;
    std::map<std::string, e_TokenType>      _mainTokenMap;
    void                                    initTokenMaps();
    public:
        Config(std::string fileName, Debug &dfile);
        Config(const Config &);
        ~Config();


        Config  &operator=(const Config &);

        bool            checkServerData(int index) const;
        void            *getServerParam(int serverID, std::string param) const;
        t_ServerData    getServerData(int serverID) const;
        void            setServerData(t_ServerData data);
        void            parseContent();

        e_LocationToken findToken();


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

#endif
