#ifndef __BEARING_H__
#define __BEARING_H__

/**
 * A kind of value which is a real number mod 2*Pi
 */

#include <Value.h>

class Bearing : public Value
{
public:

	/**
	 * A constructor. It will be used for reading from a file or from the
	 * treeview text field.
	 * @param s A textual representation of the angle in degrees.
	 */
	Bearing(const std::string &s);

	/**
	 * A constructor. It will be used for initializing the value from code.
	 * @param angle The angle in degrees.
	 */
	Bearing(const double &angle);

	/**
	 * A constructor. It will be used for initializing the value from code.
	 * It should work correctly as long as one of the values is not zero.
	 * @param rise A y delta
	 * @param run An x delta
	 */
	Bearing(const double &rise, const double &run);

	/**
	 * A destructor. Doesn't do much of anything.
	 */
	~Bearing();

	/**
	 * Return the angle with 0 indicating directly to the right
	 * and values proceeding clockwise.
	 */
	double getAngle(void) const;

	/**
	 * Determine if two bearings have equal value. There is no epsilon.
	 * Usually two values being equal imply they were derived from the
	 * same calculations. They might be equal coincidentally which is
	 * also fine.
	 * @param v A value to compare to this one.
	 */
	bool operator!=(const Value &v) const;

	/**
	 * Produce a duplicate bearing with the same value.
	 */
	Value *copy(void) const;

	/**
	 * Return the value as a string
	 */
	std::string getString(void) const;

private:
	double angle;
};

#endif  /* __BEARING_H__ */
