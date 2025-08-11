#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "StdLibs.hpp"
# include "Error.hpp"

class Config
{
    std::string _content;
    public:
        Config(std::string fileName);
        Config(const Config &);
        ~Config();

        Config  &operator=(const Config &);
};

class ConfigError : public Config, public Error
{
    public:
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
}

#endif