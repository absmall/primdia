#include <sstream>
#include <iostream>

#include <Distance.h>
#include <DistanceType.h>

using namespace std;

DistanceType::DistanceType() : Type("Distance")
{
}

Distance::Distance(const std::string &s)
{
	stringstream ss(s);
	ss >> distance;
}

Distance::Distance(const double &d)
{
	distance = d;
}

Distance::~Distance()
{
}

double Distance::getDistance(void) const
{
	return distance;
}

bool Distance::operator!=(const Value &v) const
{
	const Distance &b = static_cast<const Distance &>(v);
	return distance != b.distance;
}

Value *Distance::copy() const
{
	return new Distance(distance);
}

std::string Distance::getString(void) const
{
	stringstream ss;
	ss << distance;
	return ss.str();
}

Value *DistanceType::parse(const std::string &s) const
{
	return new Distance(s);
}
