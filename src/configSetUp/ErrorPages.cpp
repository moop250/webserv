#include "ErrorPages.hpp"

ErrorPages::ErrorPages()
{
    _html_content.push_back("UNDEFINED");
    _data.insert(std::make_pair(0, "UNDEFINED"));
}

ErrorPages::ErrorPages(std::map<int, std::string> m)
{
    _data = m;
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++)
    {
        const std::string   &filepath = i->second;
        std::ifstream       htmlFile(filepath.c_str());
        if (htmlFile.is_open())
        {
            std::string line;
            std::string buf;
            while (std::getline(htmlFile, line))
            {
                buf.append(line);
                buf.append("\n");
            }
            _html_content.push_back(buf);
            htmlFile.close();
        }
        else
            _html_content.push_back("HTML FILE EMPTY");
    }
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

/*	Error pages ANNEXE : 
	* enum RequestError
	* {
	* 	ERROR_BAD_REQUEST,
	* 	ERROR_NOT_FOUND,
	* 	ERROR_NOT_ALLOWED,
	* 	ERROR_LENGTH,
	* 	ERROR_TOO_LARGE,
	* 	ERROR_NOT_IMPLEMENTED,
	* 	ERROR_HTTP_MISSMATCH,
	* 	ERROR_INTERNAL_ERROR,
	* 	OTHER
	* };
	*
	* for nguyen
*/

std::string ErrorPages::path(RequestError error) const
{
    int fmtError = macrosLinkRequest(error);
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++)
        if (i->first == fmtError)
            return i->second;
    return "";
}

std::string ErrorPages::content(RequestError error) const
{
    int index = find(error);
    if (index == -1)
    {
        std::cerr << "Content not found\n";
        return ("");
    }
    return _html_content.at(index);
}

int     ErrorPages::macrosLinkRequest(RequestError error) const
{
    switch (error)
    {
    case REQUEST_ERROR_BAD_REQUEST:
        return BAD_REQUEST;
    case REQUEST_ERROR_NOT_FOUND:
        return NOT_FOUND;
    case REQUEST_ERROR_NOT_ALLOWED:
        return METHOD_NOT_ALLOWED;
    case REQUEST_ERROR_LENGTH:
        return LENGTH_REQUIRED;
    case REQUEST_ERROR_TOO_LARGE:
        return CONTENT_TOO_LARGE;
    case REQUEST_ERROR_NOT_IMPLEMENTED:
        return NOT_IMPLEMENTED;
    case REQUEST_ERROR_HTTP_MISSMATCH:
        return HTTP_VERSION_MISMATCH;
    case REQUEST_ERROR_INTERNAL_ERROR:
        return INTERNAL_ERROR;
    default:
        break;
    }
    return (0);
}


bool    ErrorPages::has(int error) {
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++)
        if (i->first == error)
            return true;
    return false;
}

bool ErrorPages::has(RequestError error)
{
    int fmtError = macrosLinkRequest(error);
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++)
        if (i->first == fmtError)
            return true;
    return (0);
}

int ErrorPages::find(RequestError error) const
{
    int member = 0;
    int fmtError = macrosLinkRequest(error);
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member++)
        if (i->first == fmtError)
            return member;
    return (-1);
}


int ErrorPages::find(int error) const
{
    int member = 0;
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member++)
        if (i->first == error)
            return member;
    return -1;
}