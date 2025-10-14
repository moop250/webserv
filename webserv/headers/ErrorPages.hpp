#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include "StdLibs.hpp"
# include "Request.hpp"

class ErrorPages
{
    private:
        int _nbPages;
        std::vector<std::string>    _html_content;
        std::map<int, std::string>  _data;
        int     macrosLinkRequest(RequestError error) const;
    public:
        ErrorPages();
        ErrorPages(std::map<int, std::string>);
        ErrorPages(const ErrorPages& copy);
        ErrorPages& operator=(const ErrorPages& pages);
        ~ErrorPages();

        void        setNb(int newValue);
        void        setMap(std::map<int, std::string>);
        void        setContent(std::vector<std::string>);

        void        add(int error, std::string path);
        void        add(RequestError error, std::string path);

        void        replace(int error, std::string path);
        void        replace(RequestError error, std::string path);

        int         getNbPages() const;
        int         error(int index = 0) const;
        std::string path(int index = 0) const;
        std::string content(int index = 0) const;

        std::string path(RequestError error) const;
        std::string content(RequestError error) const;

        bool        has(int error);
        bool        has(RequestError error);

        int         find(int error) const;
        int         find(RequestError error) const;        
};

#endif