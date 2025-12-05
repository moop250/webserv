#include "ErrorPages.hpp"
#include "Error.hpp"

ErrorPages::ErrorPages()
{
    const int   nbError = 12;
    std::string root = "ressources/html/error/";
    int         errors[] = {
        301, 302, 400, 403, 404, 405, 411, 413,
        415, 500, 501, 504, 505

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

void    ErrorPages::setMap(std::map<int, std::string> m)
{
    _data = m;
}

void    ErrorPages::setContent(std::vector<std::string> v)
{
    _html_content = v;
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

std::string extract_content(std::string path)
{
    std::string content = "";
    std::string line;
    std::ifstream   stream(std::string("ressources/" + path).c_str());

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            content.append(line);
            content.append("\n");
        }
    }
    return content;
}

void    ErrorPages::replace(int error, std::string path)
{
    int pos = this->find(error);

    if (pos == -1)
        return this->add(error, path);

    std::map<int, std::string>  new_map;
    std::vector<std::string>    new_vector;

    for (std::map<int, std::string>::const_iterator i = this->_data.begin(); i != this->_data.end(); i++)
    {
        if (i->first == error)
        {
            new_vector.push_back(extract_content(path));
            new_map.insert(std::make_pair(error, path));
        }
        else
        {
            new_vector.push_back(extract_content(i->second));
            new_map.insert(std::make_pair(i->first, i->second));
        }
    }

    this->_data = new_map;
    this->_html_content = new_vector;
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
    std::cout << ROSE << "[DEBUG]       : "<< WHITE << "Error attribut not found in curent server\n";
    return 0;
}

std::string ErrorPages::path(int member) const {
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++)
        if (!member)
            return i->second;
    std::cout << ROSE << "[DEBUG]       : "<< WHITE << "Error page path not found\n";
    return "";
}

std::string ErrorPages::content(int member) const {
    int index = 0;
    for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, index++)
        if (member == i->first)
            return _html_content.at(index);
    std::cout << ROSE << "[DEBUG]       : "<< WHITE << "Error page content not found\n";
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
    std::cout << ROSE << "[DEBUG]       : "<< WHITE << "Error page path not found\n";
    return "";
}

std::string ErrorPages::content(RequestError error) const
{
    int index = find(error);
    if (index == -1 ||  static_cast<size_t>(index) >= _html_content.size())
    {
        std::cout << ROSE << "[DEBUG]       : "<< WHITE << "Error page content not found\n";
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
        case REQUEST_ERROR_GATEWAY_TIMEOUT:
            return 504;
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

bool    ErrorPages::has(RequestError error)
{
    int fmtError = macrosLinkRequest(error);
    for (std::map<int, std::string>::iterator i = _data.begin(); i != _data.end(); i++)
        if (i->first == fmtError)
            return true;
    return (0);
}


int     ErrorPages::find(int error) const
{
    int index = 0;
    for (std::map<int, std::string>::const_iterator it = _data.begin();
         it != _data.end(); ++it, ++index)
    {
        if (it->first == error)
            return index;
    }
    return -1;
}

// int ErrorPages::find(RequestError error) const
// {
//     int fmtError = macrosLinkRequest(error);
//     return this->find(fmtError);
//    // for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member++)
//    //     if (i->first == fmtError)
//    //         return member;
//    // return (-1);
// }

int ErrorPages::find(RequestError error) const
{
    int fmtError = macrosLinkRequest(error);

    if (fmtError == 0)            // cas REQUEST_ERROR_OTHER ou erreur
        return -1;

    return find(fmtError);
}

// int ErrorPages::find(int error) const
// {
//     int member = 0;
//     for (std::map<int, std::string>::const_iterator i = _data.begin(); i != _data.end(); i++, member++)
//     {
//         if (i->first == error)
//             return member;
// //        std::cout << "I->first : " << i->first << " and error is  : " << error << std::endl;
//     }
//     return -1;
// }

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
