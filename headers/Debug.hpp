#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <ostream>
# include <fstream>
# include <string>
# include <iostream>
# include <sstream>
# include "Colors.hpp"
# include <time.h>

class Debug
{
    public:
        Debug();
        ~Debug();
        
        void    append(const char *msg);
        std::ofstream   file;
};


#endif