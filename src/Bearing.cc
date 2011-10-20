#include <math.h>
#include <sstream>
#include <iostream>

#include <Bearing.h>
#include <BearingType.h>

BearingType::BearingType() : Type("Bearing")
{
}

Bearing::Bearing(const std::string &s)
{
	std::stringstream ss(s);
	ss >> angle;
	angle *= M_PI/180.0;
}

Bearing::Bearing(const double &angle)
{
	this->angle = angle;
	this->angle *= M_PI/180.0;
}

Bearing::Bearing(const double &rise, const double &run)
{
	angle = atan2(rise,run);
}

Bearing::~Bearing()
{
}

double Bearing::getAngle(void) const
{
	return angle;
}

bool Bearing::operator!=(const Value &v) const
{
	const Bearing &b = static_cast<const Bearing &>(v);
	return angle != b.angle;
}

Value *Bearing::copy(void) const
{
	Bearing *b = new Bearing(angle);
	return b;
}

std::string Bearing::getString(void) const
{
	double d = angle*180.0/M_PI;
	std::ostringstream ss;
	ss << d;
	return ss.str();
}

Value *BearingType::parse(const std::string &s) const
{
	return new Bearing(s);
}
