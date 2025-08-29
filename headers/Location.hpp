#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "StdLibs.hpp"
# include "Config.hpp"
# include "ErrorPages.hpp"

//template <typename Token>
class Location
{
    protected:
        std::string                         _path;
        std::map<std::string, std::string>  _cgi;   // extension:path
        ErrorPages                          _errorPages;    // class
        std::map<int, std::string>          _error_pages;  // nb:path
        std::vector<std::string>            _methods;
        std::string                         _root;
        std::string                         _index;
        std::string                         _upload_storage;
        size_t                              _client_max_size;
        bool                                _autoindex;
    public:
        std::string foo;
        Location();
        Location(t_Location loc);
        Location(t_ServerData serv);
        Location(const Location &);
        ~Location();

        Location    &operator=(const Location &);
    
    //    virtual Token       attribut(std::string tokenType, int member = 0);    //  find through a switch case after levenstein

        //  getters
        std::string path() const;
    
        virtual std::map<std::string, std::string> cgi() const;
        virtual ErrorPages                         errorPages() const;

        virtual std::vector<std::string>    methods() const;
    
        virtual std::string root() const;
        virtual std::string index() const;
        virtual std::string storage() const;
        virtual size_t      clientSize() const;
        virtual bool        autoindex() const;

        virtual bool        undefined(e_TokenType type);
        virtual bool        has(std::string token, e_TokenType type);// = TOKEN_TYPE_COUNT);
};

//template <typename Token>
std::ostream &operator<<(std::ostream &stream, const Location &);


#endif