#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * Value which stores an RGBA color
 */

#include <Value.h>

class Color : public Value
{
public:
	/**
	 * Color constructor. Takes red, green, blue and alpha values for initialization.
	 * @param r Red value from 0 to 255
	 * @param g Green value from 0 to 255
	 * @param b Blue value from 0 to 255
	 * @param a Alpha value from 0 to 255
	 */
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	/**
	 * Read in a color in the HTML # FFFFFFFF format
	 * @param s A textual description of the color
	 */
	Color(const std::string &s);

	/**
	 * Default destructor
	 */
	~Color();

	/**
	 * Return the red component of the color.
	 * @return A value from 0.0 to 1.0
	 */
	double getRed(void) const;

	/**
	 * Return the green component of the color.
	 * @return A value from 0.0 to 1.0
	 */
	double getGreen(void) const;

	/**
	 * Return the blue component of the color.
	 * @return A value from 0.0 to 1.0
	 */
	double getBlue(void) const;

	/**
	 * Return the alpha component of the color.
	 * @return A value from 0.0 to 1.0
	 */
	double getAlpha(void) const;

	/**
	 * Return whether two colors have equal values
	 * @param v A value to compare to this one
	 * @return true if the values have a difference in any component,
	 *         false if all components are equal.
	 */
	bool operator != (const Value &v) const;

	/**
	 * Return a newly allocated color with identical value to this one.
	 * @return The copy
	 */
	Value *copy() const;

	/**
	 * Return a textual description of the color
	 * @return The color in html color reference format
	 */
	std::string getString(void) const;

private:
	/**
	 * The red component of the color
	 */
	unsigned char red;

	/**
	 * The green component of the color
	 */
	unsigned char green;

	/**
	 * The blue component of the color
	 */
	unsigned char blue;

	/**
	 * The alpha component of the color
	 */
	unsigned char alpha;
};

#endif /* __COLOR_H__ */
