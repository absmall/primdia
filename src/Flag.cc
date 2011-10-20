#include <string.h>

#include <Flag.h>
#include <FlagType.h>

using namespace std;

FlagType::FlagType() : Type("Flag")
{
}

Flag::Flag(bool status)
{
	this->status = status;
}

Flag::Flag(const std::string &s)
{
	if (!strcmp(s.c_str(), "true"))
	{
		status = true;
	} else {
		status = false;
	}
}

Flag::~Flag()
{
}

bool Flag::getValue(void) const
{
	return status;
}

bool Flag::operator!=(const Value &v) const
{
	const Flag &f = static_cast<const Flag &>(v);
	return status != f.status;
}

Value *Flag::copy() const
{
	Flag *f = new Flag(status);
	return f;
}

std::string Flag::getString(void) const
{
	if (status)
	{
		return std::string("true");
	} else {
		return std::string("false");
	}
}

Value *FlagType::parse(const std::string &s) const
{
	return new Flag(s);
}
