#include "Colors.hpp"

Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	a = 255;
	isReset = false;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->isReset = false;
}

Color Color::Reset()
{
    Color reset;
    reset.isReset = true;
    return reset;
}

std::string Color::getANSICode() const
{
    if (isReset)
        return "\033[0m";
    else
	{
        std::ostringstream oss;
        oss << "\033[38;2;" << int(r) << ";" << int(g) << ";" << int(b) << "m";
        return oss.str();
    }
}

std::string Color::blue()
{
    return "\e[0;34m";
}

std::string Color::red()
{
    return "\e[0;31m";
}

std::string Color::green()
{
    return "\e[0;32m";
}

std::string Color::yellow()
{
    return "\e[0;33m";
}

std::string Color::purple()
{
    return "\e[0;35m";
}

std::string Color::cyan()
{
    return "\e[0;36m";
}

std::string Color::white()
{
    return "\e[0;37m";
}

std::string Color::black()
{
    return "\e[0;30m";
}

std::string Color::reset()
{
    return "\e[0m";
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    return os << color.getANSICode();
}

const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);
const Color YELLOW(255, 255, 0);
const Color CYAN(0, 255, 255);
const Color ROSE(255, 0, 255);
const Color WHITE(255, 255, 255);

const Color RESET = Color::Reset();
