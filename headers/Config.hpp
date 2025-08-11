#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Debug.hpp"
# include <vector>

# define NUM_MAIN_TOKENS 8
# define NUM_LOC_TOKENS 7

typedef enum LocationTokens
{
    METHODS,                // 0
    AUTOINDEX,              // 1
    SPEC_INDEX,             // 2
    RETURN,                 // 3
    ROOT,                   // 4
    CGI_PATH,               // 5
    CGI_EXT,                // 6
    OTHER_LOCATION_TOKEN    // 7
}   e_LocationToken;

typedef enum TokenTypes
{
    LISTENER,               // 0
    SERVER_NAME,            // 1
    HOST,                   // 2
    ROOT_PATH,              // 3
    CLIENT_BUFFER_SIZE,     // 4
    HTLM_INDEX,             // 5
    ERROR_PAGE,             // 6
    LOCATION,               // 7
    OTHER_TOKEN             // 8
}   e_TokenType;

typedef struct Location
{
    std::string loc;
    std::string methods[3];
    std::string HTLMindexName;
    std::string cgi_path;
    std::string cgi_ext;
    bool        autoIndex;
}   t_Location;

typedef struct ServerData
{
    t_Location      *locations;
    std::string     tokens[NUM_MAIN_TOKENS];
}   t_ServerData;

class Config
{
//    std::vector<ServerData> *_servers;    // do cpp08
    t_ServerData            *_servers;
    Debug                   *_dfile;
    std::string             _MainTokens[NUM_MAIN_TOKENS];
    std::string             _LocTokens[NUM_LOC_TOKENS];
    std::string             _content;
    e_LocationToken         LocIndex;
    e_TokenType             TokIndex;
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