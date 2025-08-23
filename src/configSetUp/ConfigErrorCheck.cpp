#include "ConfigError.hpp"

e_lineType    getLineType(std::string line)
{
    size_t      pos = 0, pos2 = 0;
    static const char *tokens[7] = {
        "location", "server", "{", "}", "#", "<", ""
    };
    
 //   std::cout << "Line is : " << line << '\n';
    for (int type = 0; type < LINE_EMPTY; type++)
    {
        pos = line.find(tokens[type]);
        for (int is_too = 0; is_too < LINE_EMPTY; is_too++)
        {
            pos2 = line.find(tokens[is_too]);
            if (pos2 < pos)
                return (static_cast<e_lineType>(is_too));
        }
        if (pos != std::string::npos)
            return (static_cast<e_lineType>(type));
    }
    return LINE_EMPTY;
}

static void compare(const char *s1, const char *s2, unsigned int &diff)
{
    int len1 = 0, len2 = 0;

    while (s1[len1] || s2[len2])
    {
        diff += (s1[len1] - s2[len2]);
        if (s1[len1])
            len1++;
        if (s2[len2])
            len2++;
    }
}

void    ConfigError::suggsestToken(std::string pseudoToken)
{
    unsigned int  diff = 0, lessDif = 1000000;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
    {
        diff = 0;
        compare(pseudoToken.c_str(), _Tokens[i].c_str(), diff);
        std::cout <<  "lessdiff : " << lessDif << "\nDiff : " << diff << std::endl;
        if (lessDif > diff)
        {
            lessDif = diff;
            _suggsestedToken = _Tokens[i];
        }
    }
}

void    ConfigError::explicitTheError()
{
    switch (_error)
    {
        case ERROR_BRACKET:
            std::cerr << "Bracket bad\n";
            break ;
        case ERROR_NB_SERVER:
            std::cerr << "No server\n";
            break ;
        case ERROR_LINE_FORMAT:
            switch (_fmtError)
            {
                case FMT_EOF:
                    std::cerr << "fmt eof\n";
                    break ;
                case FMT_TOKEN:
                //    _suggsestedToken = suggsestToken(_Tokens, _errorLine);
                    std::cerr << "Did you mean : "
                        << GREEN << _suggsestedToken << "?" << RESET << std::endl;
                    break ;
                case FMT_BRACKET:
                    std::cerr << "fmt bracket\n";
                    break ;
                default:
                    break ;
            }
            break ;
        case ERROR_TOKENS:
            std::cerr << "token error, token needed\n";
            break ;
        default:
            break ;
    }
    return ;
}

bool ConfigError::checkBrackets()
{
    int open = 0, close = 0;
    size_t  len = _content.length();

//    std::cout << "In check brackets\n";
    for (size_t i = 0; i < len; i++)
    {
        if (_content[i] == '{')
            open++;
        if (_content[i] == '}')
            close++;
    }
    if (open != close)
        _isValid = 0;
    return OK;
}

bool    ConfigError::checkNbServers()
{
//    std::cout << "In nb servers check\n";
    if (_nbServers <= 0)
        return KO;
    return OK;
}

bool    ConfigError::checkTokens()
{
    t_ServerData    serv;

//    std::cout << "In checktokens\n";
    for (int i = 0; i < _nbServers; i++)
    {
        serv = _servers.at(i);
        if (serv.host == "UNDEFINED" || serv.port == "UNDEFINED")
            return (KO);    //  add here if more imperative tokens
    }
    return (OK);
}

bool    ConfigError::checkLine()
{
    size_t          endTok;
    std::string     pseudoToken;

    switch (_line)
    {
        case LINE_LOC:
            //  loc format
            break ;
        case LINE_SERV:
            // serv format
            break ;
        case LINE_BRACK1:
            //  brac
            break ;
        case LINE_BRACK2:
            break ;
        case LINE_COMMENT:
            // normally ok
            break ;
        case LINE_TOK:
            for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
                if (_errorLine.find(_Tokens[i]) != std::string::npos)
                    return (OK);
            while (endTok < _errorLine.length() && _errorLine[endTok] != '>')
                endTok++;
            pseudoToken = _errorLine.substr(_errorLine.find("<"), endTok);
            suggsestToken(pseudoToken);
            std::cout << _suggsestedToken << "HELLO\n" <<std::endl;
            return KO;
            //  format or chec
            break ;
        case LINE_EMPTY:
            // find suggsested
            break ;
        default:
            if (!OK)
                _fmtError = _line;
            break ;
    }
    return (OK);
}

bool    ConfigError::checkLinesFormat()
{
    std::stringstream   content;
    static const char *tokens[7] = {
        "location", "server", "{", "}", "#", "<", ""
    };

    (void)tokens;
    content << _content;
    while (std::getline(content, _errorLine) && _isValid)
    {
        _lineCount++;
        _fmtError = 0;
        _line = getLineType(_errorLine);
//        std::cout << "Line type is : " << tokens[_line] << '\n';
        if (!checkLine())
        {
            //find
            return (KO);
        }
    }
//    std::cout << "In checklineformat\n"; 
    return (OK);
}

bool    ConfigError::checkConfig()
{
    bool    status;

    _error = 0;
    if (_content.empty())
        Error("empty file");
    for (; _error < CONFIG_CHECKERS && _isValid; _error++)
        if (!(status = (this->*checkers[_error])()))
            _isValid = KO;
    if (_isValid)
        return OK;
    Error(errors[_error].c_str(), "Server N* i", "File.config", _lineCount);
    explicitTheError();
    return (KO);
}
