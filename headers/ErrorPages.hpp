#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include "StdLibs.hpp"
# include "Request.hpp"

class ErrorPages
{
    public:
        ErrorPages();
        ErrorPages(std::map<int, std::string>);
        ~ErrorPages();

        int         error(int index = 0) const;
        std::string path(int index = 0) const;
        std::string content(int index = 0) const;

        int         error(RequestError error) const;
        std::string path(RequestError error) const;
        std::string content(RequestError error) const;

        bool        has(int error);
        bool        has(RequestError error);

        int         find(int error);
        int         find(RequestError error);
    private:
        std::vector<std::string>    _html_content;
        std::map<int, std::string>  _data;
        int     macrosLinkRequest(RequestError error) const;
        
};

#endif