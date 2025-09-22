#include "nginxManip.hpp"

enum {
    FLAG_UP,
    FLAG_DOWN,
    FLAG_AFTER,
    FLAG_OTHER
};

static void describe_use()
{
    std::cout << "Use : "
        << "./insert --[OPTIONS] ... [TEXT] [FILE]\n"
        << "example : ./insert --after http 'blablabla' in /etc/nginx/nginx.conf"
}

void    parse_flags()
{
    std::string flags[] = {
        "--up", "--down", "--after", "--",
        "--", "--", "--", "--",
        "--", "--", "--"
    };
    std::string use[] = {
        "--up", "--down", "--after 'n-ieme occurance' ''"
    };
}

//  use insert
int main(int ac, char **av) // av[1] = file 
{
    if (ac != 3)
    {
        describe_use();
        return 1;
    }
    smanip::insert(av[1]);
    return (0);
}

//  use remove
int main(int ac, char **av)
{
    return (0);
}