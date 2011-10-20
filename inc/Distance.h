#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <config/common.h>

#include <Value.h>

class Distance : public Value
{
public:
	Distance(const double &d);
	Distance(const std::string &s);
	~Distance();


	/**
	 * Return the value of this distance.
	 */
	double getDistance(void) const;

	/**
	 * Determine if two distances have equal value. There is no epsilon.
	 * Usually two values being equal imply they were derived from the
	 * same calculations. They might be equal coincidentally which is
	 * also fine.
	 */
	bool operator!=(const Value &v) const;

	/**
	 * Produce a duplicate distance with the same value.
	 */
	Value *copy() const;

	/**
	 * Return the value as a string
	 */
	std::string getString(void) const;

private:
	double distance;
};

#endif /* __DISTANCE_H__ */
