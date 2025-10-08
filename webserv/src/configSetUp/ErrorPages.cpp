#include "ErrorPages.hpp"
#include "Error.hpp"

ErrorPages::ErrorPages()
{
    const int   nbError = 12;
    std::string root = "ressources/html/error/";
    int         errors[] = {
        301, 302, 400, 403, 404, 405, 411, 413,
        415, 500, 501, 505
    };

    _nbPages = 0;

    for (int i = 0; i < nbError; i++)
    {
        _data.insert(std::make_pair(errors[i], tostring(errors[i]) + ".html"));
        std::string input = root + tostring(errors[i]) + std::string(".html");
        std::ifstream   error(input.c_str());
        std::string     content = "";
        std::string     buf;

        while (std::getline(error, buf))
        {
            content.append(buf);
            content.append("\n");
        }
        if (error.is_open())
            _html_content.push_back(content);
        else
            std::cout << "Content not found\n" << std::flush;
        _nbPages++;
    }       
}

ErrorPages::ErrorPages(std::map<int, std::string> m)
{
    _data = m;
    _nbPages = 0;
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
        _nbPages += 1;
    }
}

ErrorPages::ErrorPages(const ErrorPages& copy) {
    this->_html_content = copy._html_content;
    this->_data = copy._data;
    this->_nbPages = copy._nbPages;
}
ErrorPages& ErrorPages::operator=(const ErrorPages& pages) {
    if (this != &pages) {
        this->_html_content = pages._html_content;
        this->_data = pages._data;
        this->_nbPages = pages._nbPages;
    }
    return *this;
}

ErrorPages::~ErrorPages() {
    return ;
}

void        ErrorPages::setNb(int newValue)
{
    this->_nbPages = newValue;
}


void    ErrorPages::add(int error, std::string path)
{
    std::cout << "Path is : " << path << std::endl;
    std::ifstream   stream(std::string("ressources/" + path).c_str());
    std::string     content = "";
    std::string     line;

    if (!stream.is_open())
    {
        Error("Unvalid path", __func__, "ErrorPages::add(int, std::string)", __LINE__);
        return ;
    }

    this->_data.insert(std::make_pair(error, path));    

    while (std::getline(stream, line))
    {
        content.append(line);
        content.append("\n");
    }

    // std::cout << "CONTENT : " << content;
    this->_html_content.push_back(content);
    this->setNb(_nbPages + 1);

    return ; (void)error;
}

void    ErrorPages::add(RequestError error, std::string path)
{
    this->add(macrosLinkRequest(error), path);
}

void    ErrorPages::replace(int error, std::string path)
{
    int pos = this->find(error);

    std::cout << GREEN << "REPLACE !" << RESET << std::endl;
    if (pos == -1)
        return this->add(error, path);

    this->_data[error] = path;
    std::string content = "";
    std::string line;
    std::cout << "path is : " << path << std::endl;
    std::ifstream   stream(std::string("ressources/" + path).c_str());

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            content.append(line);
            content.append("\n");
        }
    }
    
    std::cout <<  "\n\n YAYYYY\n\n" << content;
    this->_html_content[pos] = content;
    return ;
}

void    ErrorPages::replace(RequestError error, std::string path)
{
    this->replace(macrosLinkRequest(error), path);
}


int ErrorPages::getNbPages() const { return _nbPages; }

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
    int index = 0;
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, index++)
        if (member == i->first)
            return _html_content.at(index);
    return ("");
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
    switch (error) {
        case REQUEST_ERROR_MOVED_PERMANENTLY:
            return 301;
        case REQUEST_ERROR_FOUND:
            return 302;
        case REQUEST_ERROR_BAD_REQUEST:
            return 400;
        case REQUEST_ERROR_FORBIDDEN:
            return 403;
        case REQUEST_ERROR_NOT_FOUND:
            return 404;
        case REQUEST_ERROR_METHOD_NOT_ALLOWED:
            return 405;
        case REQUEST_ERROR_LENGTH_REQUIRED:
            return 411;
        case REQUEST_ERROR_CONTENT_TOO_LARGE:
            return 413;
        case REQUEST_ERROR_UNSUPPORTED_MEDIA_TYPE:
            return 415;
        case REQUEST_ERROR_INTERNAL_ERROR:
            return 500;
        case REQUEST_ERROR_NOT_IMPLEMENTED:
            return 501;
        case REQUEST_ERROR_HTTP_VERSION_MISMATCH:
            return 505;
        case REQUEST_ERROR_OTHER:
            break;
        default:
            break ;
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

//std::ostream    &operator<<(std::ostream &stream, const ErrorPages &p)
//{
//    for (int i = 0;)
//    return stream;
//}
template <typename integer>
static void	helper(integer nb, char str[100], int *index)
{
	if (nb >= 10)
		helper(nb / 10, str, index);
	str[(*index)++] = (nb % 10) + '0';
	str[*index] = '\0';
}

template <typename integer>
std::string	tostring(integer n)
{
    char        c[100];
	int		    index;
    std::string s;

    for (int i = 0; i < 100; i++)
	{
        c[i] = '\0';
	}
	index = 0;
	if (n == -2147483648)
	{
		c[0] = '-';
		c[1] = '2';
		n += 2000000000;
		n = -n;
		index += 2;
	}
	else if (n < 0)
	{
		c[index++] = '-';
		n = -n;
	}
    helper(n, c, &index);
    s = c;
	return (s);
}
