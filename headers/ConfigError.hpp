#ifndef CONFIGERROR_HPP
# define CONFIGERROR_HPP

# include "Config.hpp"

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
        bool        checkContent();
        int         getLineType();
    private:    //  main checkers
        bool        checkBrackets();
        bool        checkNbServers();
        bool        checkLinesFormat();
        bool        checkTokens();
                //  error checkers
        bool        eof();
        bool        token();
        bool        bracket();
        bool        foo();
    private:    //  util type var
        e_lineType  line;
        bool        (*checkers[5])();
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