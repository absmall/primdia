#ifndef __POINT2D_H__
#define __POINT2D_H__

#include <Value.h>

class Point2D : public Value
{
public:
	Point2D();
	Point2D(const std::string &s);
	Point2D(const double &x, const double &y);
	~Point2D();
	double getX() const;
	double getY() const;
	bool operator!=(const Value &v) const;
	Value *copy() const;

	/**
	 * Return the value as a string
	 */
	std::string getString(void) const;

private:
	double x;
	double y;
};

#endif /* __POINT2D_H__ */
