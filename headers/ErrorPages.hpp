#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include "StdLibs.hpp"
# include "Request.hpp"

class ErrorPages
{
    public:
        ErrorPages();
        ErrorPages(std::map<int, std::string>);
        ErrorPages(const ErrorPages& copy);
        ErrorPages& operator=(const ErrorPages& pages);
        ~ErrorPages();

        int         error(int index = 0) const;
        std::string path(int index = 0) const;
        std::string content(int index = 0) const;

        std::string path(RequestError error) const;
        std::string content(RequestError error) const;

        bool        has(int error);
        bool        has(RequestError error);

        int         find(int error) const;
        int         find(RequestError error) const;
    private:
        std::vector<std::string>    _html_content;
        std::map<int, std::string>  _data;
        int     macrosLinkRequest(RequestError error) const;
        
};

#endif