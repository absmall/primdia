#ifndef __LABELTYPE_H__
#define __LABELTYPE_H__

#include <config/common.h>

#include <Type.h>

/**
 * The type for text strings.
 */

class LabelType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	LabelType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the bearing.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __LABELTYPE_H__ */
