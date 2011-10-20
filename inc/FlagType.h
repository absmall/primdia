#ifndef __FLAGTYPE_H__
#define __FLAGTYPE_H__

/**
 * A type for a boolean flag. It parses the two strings:
 * `true' and `false'.
 */

#include <config/common.h>

#include <Type.h>

class FlagType : public Type
{
public:
	/**
	 * Default constructor.
	 */
	FlagType();

	/**
	 * Parse a value out of a string. This is intended
	 * to be used by the tools.
	 * @param s The string representation of the flag.
	 */
	Value *parse(const std::string &s) const;
};

#endif /* __FLAGTYPE_H__ */
