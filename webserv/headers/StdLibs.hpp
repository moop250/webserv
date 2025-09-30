#ifndef STDLIBS_HPP
# define STDLIBS_HPP

# include <iostream>
# include <string>
# include <ostream>
# include <fstream>
# include <list>
# include <string>
# include <vector>
# include "Colors.hpp"
# include <stdint.h>
# include <vector>
# include <map>

# include <cstdlib>

template <typename integer>
std::string	tostring(integer n);


int levenshtein(const std::string& s1, const std::string& s2);

#endif
