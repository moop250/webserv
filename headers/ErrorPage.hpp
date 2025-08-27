#ifndef ERRORPAGE_HPP
# define ERRORPAGE_HPP

# include "StdLibs.hpp"

class ErrorPage
{
    public:
        ErrorPage();
        ErrorPage(std::map<int, std::string>);
        ~ErrorPage();

        int         error(int member) const;
        std::string path(int member) const;
        std::string content() const;

        bool        has(int error);
    private:
        std::string                 _html_content;
        std::map<int, std::string>  _data;
};

#endif