#include <stdio.h>
#include <sstream>

#include <Point2D.h>
#include <Point2DType.h>

using namespace std;

Point2DType::Point2DType() : Type("2D Point")
{
}

Point2D::Point2D()
{
}

Point2D::Point2D(const std::string &s)
{
	sscanf(s.c_str(), "(%lg, %lg)", &x, &y);
}

Point2D::Point2D(const double &x, const double &y)
{
	this->x = x;
	this->y = y;
}

Point2D::~Point2D()
{
}

double Point2D::getX() const
{
	return x;
}

double Point2D::getY() const
{
	return y;
}

bool Point2D::operator!=(const Value &v) const
{
	const Point2D &p2d = static_cast<const Point2D &>(v);
	if (x != p2d.x || y != p2d.y)
	{
		return true;
	} else {
		return false;
	}
}

Value *Point2D::copy() const
{
	return new Point2D(x, y);
}

std::string Point2D::getString(void) const
{
	std::ostringstream ss;
	ss << "(" << x << ", " << y << ")";
	return ss.str();
}

Value *Point2DType::parse(const std::string &s) const
{
	return new Point2D(s);
}
