#ifndef __POINT2DTYPE_H__
#define __POINT2DTYPE_H__

/**
 * The type for a 2D point on the canvas. It parses descriptions
 * of the form (x,y)
 */

#include <config/common.h>

#include <Type.h>

class Point2DType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	Point2DType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the point.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __POINT2DTYPE_H__ */
