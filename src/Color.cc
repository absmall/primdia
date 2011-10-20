#include <ctype.h>
#include <string.h>
#include <iostream>

#include <Color.h>
#include <ColorType.h>

using namespace std;

static inline int hex(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c-'0';
	} else {
		return toupper(c)-'A'+10;
	}
}

static inline char tohex(int i)
{
	if (i <= 9)
	{
		return i+'0';
	} else {
		return i-10+'A';
	}
}

ColorType::ColorType() : Type("Color")
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

Color::Color(const std::string &s)
{
	bool invalid = false;

	if (s.length() != 9 || s[0] != '#')
	{
		invalid = true;
	}

	for(int i=1; i < 9; i++)
	{
		if (!((s[i] >= '0' && s[i] <= '9') || (toupper(s[i]) >= 'A' && toupper(s[i]) <= 'F')))
		{
			invalid = true;
		}
	}

	if (invalid)
	{
		red = 0;
		green = 0;
		blue = 0;
		alpha = 0;
		return;
	}

	red = hex(s[1])*16+hex(s[2]);
	green = hex(s[3])*16+hex(s[4]);
	blue = hex(s[5])*16+hex(s[6]);
	alpha = hex(s[7])*16+hex(s[8]);
}

Color::~Color()
{
}

double Color::getRed(void) const
{
	return red/255.0;
}

double Color::getGreen(void) const
{
	return green/255.0;
}

double Color::getBlue(void) const
{
	return blue/255.0;
}

double Color::getAlpha(void) const
{
	return alpha/255.0;
}

bool Color::operator!=(const Value &v) const
{
	const Color &f = static_cast<const Color &>(v);
	if (red != f.red) return false;
	if (green != f.green) return false;
	if (blue != f.blue) return false;
	if (alpha != f.alpha) return false;
	return true;
}

Value *Color::copy() const
{
	Color *f = new Color(red, green, blue, alpha);
	return f;
}

std::string Color::getString(void) const
{
	std::string s = "#";

	s.push_back(tohex(red/16));
	s.push_back(tohex(red%16));
	s.push_back(tohex(green/16));
	s.push_back(tohex(green%16));
	s.push_back(tohex(blue/16));
	s.push_back(tohex(blue%16));
	s.push_back(tohex(alpha/16));
	s.push_back(tohex(alpha%16));

	return s;
}

Value *ColorType::parse(const std::string &s) const
{
	return new Color(s);
}
