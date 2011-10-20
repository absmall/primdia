#ifndef __DISTANCETYPE_H__
#define __DISTANCETYPE_H__

/**
 * The type for a distance value. It parses real numbers
 * in UTF-8 strings.
 */

#include <config/common.h>

#include <Type.h>

class DistanceType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	DistanceType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the bearing.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __DISTANCETYPE_H__ */
