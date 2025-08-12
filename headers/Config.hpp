#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
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
    OTHER_TOKEN,
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
    OTHER_LOCATION_TOKEN,
    LOCATION_TOKEN_COUNT    //                          9
} e_LocationToken;

# define NUM_MAIN_TOKENS TOKEN_TYPE_COUNT
# define NUM_LOC_TOKENS LOCATION_TOKEN_COUNT


typedef struct Location
{
    std::string                 loc;            // path of location
    std::vector<std::string>    methods;        // GET, POST, etc.
    std::string                 redirection;    // could be "301 /newpath"
    std::string                 root;           // path mapping for this location
    bool                        autoIndex;      // true/false
    std::string                 defaultFile;    // index.html
    std::string                 uploadDir;      // upload storage
    std::string                 cgi_ext;        // .php, .py
    std::string                 cgi_path;       // path to interpreter
} t_Location;

typedef struct ServerData
{
    std::vector<std::string>    listeners;          // multiple host:port
    std::string                 server_name;
    std::map<int, std::string>  error_pages;        // code -> file path
    size_t                      client_max_body_size;
    std::string                 root;
    std::vector<t_Location>     locations;
} t_ServerData;

class Config
{
    Debug                                   *_dfile;
    t_ServerData                            *_servers;
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
        t_ServerData    getServerData(int index) const;
        void            setServerData(t_ServerData data);
        void            parseContent();

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
