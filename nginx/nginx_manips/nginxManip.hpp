#ifndef NGINXMANIP_HPP
# define NGINXMANIP_HPP

# include <ostream>
# include <fstream>
# include <iostream>
# include <string>
# include <sstream>

class smanip
{
	private:
		smanip();
		~smanip();

	public:
		//		stream manipulation methods
		static std::string	content(std::string file);
	
		static void		insert(
			std::string in_file, size_t at, std::string what
		);

		static void		remove(
			std::string in_file, size_t at, std::string what
		);
};

#endif