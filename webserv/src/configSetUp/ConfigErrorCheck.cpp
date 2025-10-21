#include "ConfigError.hpp"
#include "Server.hpp"

e_lineType    getLineType(std::string line)
{
    size_t      pos = 0, pos2 = 0;
    static const char *tokens[7] = {
        "location", "server", "{", "}", "#", "<", ""
    };
    
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

template <typename x>
static x    min(const x &a, const x &b)
{
    return a < b ? a : b;
}

int levenshtein(const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();
    std::vector<std::vector<int> > dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i)
        dp[i][0] = i;
    for (int j = 0; j <= n; ++j)
        dp[0][j] = j;

    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
            {
                dp[i][j] = 1 + min(
                    dp[i - 1][j],               // suppression
                    min(
                        dp[i][j - 1],           // insertion
                        dp[i - 1][j - 1]        // substitution
                    )
                );
            }
        }
    }
    return dp[m][n];
}

void    ConfigError::suggsestToken(std::string pseudoToken)
{
    unsigned int  diff = 0, lessDif = 1000000;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++)
    {
        diff = levenshtein(pseudoToken, _Tokens[i]);
        if (lessDif > diff)
        {
            lessDif = diff;
            _suggsestedToken = _Tokens[i];
        }
    }
}

void    ConfigError::explicitTheError()
{
    _error--;
    sanitizeLine(_errorLine);
    std::cout << ROSE << "Line concerned : " << _errorLine << RESET << '\n';
    switch (_error)
    {
        case ERROR_BRACKET:
            std::cerr << CYAN << "Unmatched Brackets\n" <<RESET;
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
                    suggsestToken(_errorLine);
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
    if (open != close || !open)
        _isValid = 0;
    return OK;
}

bool    ConfigError::checkNbServers()
{
    if (_nbServers <= 0)
        return KO;
    return OK;
}

bool    ConfigError::checkTokens()
{
    for (int j = 0; j < _nbServers; j++)
    {
        Server  s(_servers.at(j));
        for (int i = 0; i < _nbServers; i++)
        {
            Server  s2(_servers.at(i));
            if (s2.undefined(HOST) || s2.undefined(LISTEN) || s2.undefined(SERVER_NAME))
            {
                std::cerr << "host or port not defined in server\n";
                return (KO);    //  add here if more imperative tokens
            }
            if (i != j && (s.port() == s2.port()))// || s.name() == s2.name()))
            {
                std::cerr << YELLOW << "[WARNING]   : " << RESET << "Two servers share the same port\n\n";
                return (KO);	// False initialement
            }
        }
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
                if (!_Tokens[i].empty() && _errorLine.find(_Tokens[i]) != std::string::npos)
                    return (OK);
            while (endTok < _errorLine.length() && _errorLine[endTok] != '>')
                endTok++;
            pseudoToken = _errorLine.substr(_errorLine.find("<"), endTok);
            suggsestToken(pseudoToken);
            _error = ERROR_LINE_FORMAT;
            _fmtError =  FMT_TOKEN;
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

    content << _content;
    _fmtError = 0;
    while (std::getline(content, _errorLine) && _isValid)
    {
        _lineCount++;
        _line = getLineType(_errorLine);
        if (!checkLine())
            return (KO);
    }
    return (OK);
}

static int countOccurence(std::string toFind, std::string in, size_t from, size_t until)
{
    int         count = 0;
    size_t      matchCursor = 0;


    if (until > in.length())
        return (0);
    for (size_t i = from; i < until; i++)
    {
        matchCursor = 0;
        while (matchCursor < toFind.length())
        {
            if (in[i + matchCursor] != toFind[matchCursor])
                break ;
            matchCursor++;
        }
        if (matchCursor >= toFind.length() - 1)
        {
            count++;
            i += matchCursor;
        }
    }
    return (count);
}

static std::string describeErrorContext(std::string content, std::string _errorLine)
{
    std::string msg = "Server '";
    int servId = 0, locId = 0;

    servId = countOccurence("server ", content, 0, content.find(_errorLine)) - 1;
    msg += static_cast<char>(servId + '0');
    msg += '\'';
    msg += " in location block nb : ";
    size_t  from = 0;
    while (servId-- > 0)
        from = content.find("server", from);
    locId = countOccurence("location ", content, from, content.find(_errorLine));
    msg += locId + '0';
    return msg;
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
    std::string describeContext = describeErrorContext(_content, _errorLine);
    Error(errors[_error].c_str(), describeContext.c_str(), "Your config file", _lineCount);
    explicitTheError();
    return (KO);
}
