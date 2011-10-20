#ifndef __BEARINGTYPE_H__
#define __BEARINGTYPE_H__

#include <config/common.h>

/**
 * The type of an angle mod 2*Pi. It parses real numbers
 * in UTF-8 strings.
 */

#include <Type.h>

class BearingType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	BearingType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the bearing.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __BEARINGTYPE_H__ */
