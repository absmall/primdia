#ifndef __FONTTYPE_H__
#define __FONTTYPE_H__

#include <config/common.h>

#include <Type.h>

/**
 * The type for font descriptors.
 */

class FontType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	FontType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the bearing.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __FONTTYPE_H__ */
