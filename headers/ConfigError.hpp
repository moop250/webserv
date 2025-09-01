#ifndef CONFIGERROR_HPP
# define CONFIGERROR_HPP

# include "Config.hpp"

# define CONFIG_CHECKERS 4
# define LINE_CHECKERS 4
# define KO 0
# define OK 1

enum e_lineType
{
    LINE_LOC,
    LINE_SERV,
    LINE_BRACK1,
    LINE_BRACK2,
    LINE_COMMENT,
    LINE_TOK,
    LINE_EMPTY,
};

enum e_config_errors
{
    ERROR_BRACKET,
    ERROR_NB_SERVER,
    ERROR_LINE_FORMAT,
    ERROR_TOKENS,
};

enum e_format_errors
{
    FMT_DOUBLE,
    FMT_EOF,
    FMT_TOKEN,
    FMT_BRACKET,
    FMT_FOO
};


class ConfigError : public Config
{
    private:    //  logic status
        std::string _suggsestedToken;
        std::string _block;
        std::string _errorLine;
        int         _lineCount;
        int         _error;
        int         _fmtError;
        bool        _isValid;
    public:     //  pivot
        bool        checkConfig();
    private:    //  config checkers
        bool        checkBrackets();
        bool        checkNbServers();
        bool        checkLinesFormat();
        bool        checkTokens();
        bool        checkLine();
    private:    //  util type var
        e_lineType  _line;
        bool        (ConfigError::*checkers[CONFIG_CHECKERS])(void);
        std::string errors[CONFIG_CHECKERS + LINE_CHECKERS];
        void        explicitTheError();
        void        suggsestToken(std::string pseudoToken);
    public:
        ConfigError();
        ConfigError(const Config &);
        ConfigError(const ConfigError &);
        ~ConfigError();

        ConfigError &operator=(const ConfigError &);

        bool        isConfigValid() const;
        std::string getErrorLine() const;

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

#endif