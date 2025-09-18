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
        Debug(std::string name = "Debug.log");
        ~Debug();
        
        void    append(const char *msg);
        std::ofstream   file;
};


#endif