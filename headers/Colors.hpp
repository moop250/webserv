#ifndef COLOR_HPP
#define COLOR_HPP

# include <iostream>
# include <string>
# include <sstream>

struct Color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
	bool			isReset;

	Color();
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	static Color	Reset();
	std::string		getANSICode() const;
};

std::ostream& operator<<(std::ostream& os, const Color& color);

extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color YELLOW;
extern const Color CYAN;
extern const Color ROSE;
extern const Color WHITE;

extern const Color RESET;

#endif
