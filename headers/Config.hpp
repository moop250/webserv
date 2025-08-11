#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Debug.hpp"

# define NUM_TOKENS 10

class Config
{
    Debug       *_dfile;
    std::string _tokens[NUM_TOKENS];
    std::string _content;
    public:
        Config(std::string fileName, Debug &dfile);
        Config(const Config &);
        ~Config();

        Config  &operator=(const Config &);

        void    unexpected();
        void    parseContent();

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
};

#endif