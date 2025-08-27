#include "ErrorPage.hpp"

ErrorPage::ErrorPage()
{
    _html_content = "UNDEFINED";
    _data.insert(std::make_pair(0, "UNDEFINED"));
}

ErrorPage::ErrorPage(std::map<int, std::string> m)
{
    _data = m;
    //  look for content;
}

ErrorPage::~ErrorPage() {
    return ;
}

int ErrorPage::error(int member) const {
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member--)
        if (!member)
            return i->first;
    return -1;
}

std::string ErrorPage::path(int member) const {
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member--)
        if (!member)
            return i->second;
    return "";
}

std::string ErrorPage::content() const { return _html_content; }

bool    ErrorPage::has(int error) {
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++)
        if (i->first == error)
            return true;
    return false;
}
