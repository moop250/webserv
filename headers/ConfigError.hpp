#ifndef CONFIGERROR_HPP
# define CONFIGERROR_HPP

# include "Config.hpp"

# define CONFIG_CHECKERS 4
# define LINE_CHECKERS 4
# define KO 0
# define OK 1
enum e_lineType
{
    SER_LINE,
    LOC_LINE,
    TOK_LINE,
    BRACKET,
    EMPTY
};

class ConfigError : public Config
{
    private:    //  logic status
        std::string _suggsestedToken;
        std::string _errorLine;
        bool        _isValid;
    public:     //  pivot
        bool        checkConfig();
        int         getLineType();
    private:    //  config checkers
        bool        checkBrackets();
        bool        checkNbServers();
        bool        checkLinesFormat();
        bool        checkTokens();
                //  line checkers
        bool        eof(std::string);
        bool        token(std::string);
        bool        bracket(std::string);
        bool        foo(std::string);
    private:    //  util type var
        e_lineType  _line;
        bool        (ConfigError::*checkers[CONFIG_CHECKERS])(void);
        bool        (ConfigError::*lineCheckers[LINE_CHECKERS])(std::string);
        std::string errors[CONFIG_CHECKERS + LINE_CHECKERS];
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