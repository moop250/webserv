#include "ErrorPages.hpp"

ErrorPages::ErrorPages()
{
    _html_content.push_back("UNDEFINED");
    _data.insert(std::make_pair(0, "UNDEFINED"));
}

ErrorPages::ErrorPages(std::map<int, std::string> m)
{
    _data = m;
    //  look for content
}

ErrorPages::~ErrorPages() {
    return ;
}

int ErrorPages::error(int member) const {
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++)
        if (!member)
            return i->first;
    return 0;
}

std::string ErrorPages::path(int member) const {
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++)
        if (!member)
            return i->second;
    return "";
}

std::string ErrorPages::content(int member) const {
    try {
        if (static_cast<size_t>(member) >= _html_content.size())
            return ("");
        return _html_content.at(member);
    } catch (std::out_of_range()) {
        std::cerr << "Out of range bitch\n";
        return ("");
    }
}

int         ErrorPages::error(RequestError error) const
{
    (void)error;
    //return _data.at(find(error));
    return 0;
}

std::string ErrorPages::path(RequestError error) const
{
    (void)error;
    return "";
}

std::string ErrorPages::content(RequestError error) const
{
    (void)error;
    return "";
}

int     ErrorPages::macrosLinkRequest(RequestError error) const
{
    switch (error)
    {
    case 0:
        /* code */
        break;
    
    default:
        break;
    }
    return (0);
}


bool    ErrorPages::has(int error) {
    int member = 0;
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++, member++)
        if (i->first == error)
            return member;
    return 0;
}

bool ErrorPages::has(RequestError error)
{
    (void)error;
    return (0);
}

int ErrorPages::find(RequestError error)
{
    (void)error;
    return (0);
}


int ErrorPages::find(int error) {
    int member = 0;
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++, member++)
        if (i->first == error)
            return member;
    return -1;
}