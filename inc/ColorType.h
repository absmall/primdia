#ifndef __COLORTYPE_H__
#define __COLORTYPE_H__

/**
 * The type of a color value. It parses numbers as a # sign
 * followed by 8 hex digits representing RGBA.
 */

#include <config/common.h>

#include <Type.h>

class ColorType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	ColorType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the bearing.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __COLORTYPE_H__ */
