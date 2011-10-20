#include <Type.h>

using namespace std;

static map<const string, Type *> &types()
{
	static map<const string, Type *> typeI;
	return typeI;
}

Type::Type(const std::string &name)
{
	this->name = name;
	registerType(name, this);
}

Type::~Type()
{

}

Type *Type::getType(const std::string &s)
{
	map<const std::string, Type *>::iterator i = types().find(s);
	if (i == types().end()) throw s+": Invalid type";
	return types().find(s)->second;
}

void Type::registerType(const std::string &name, Type *description)
{
	types()[name] = description;
}
