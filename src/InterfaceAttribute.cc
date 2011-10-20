#include <InterfaceAttribute.h>

InterfaceAttribute::InterfaceAttribute(Interface *i, const Type *t, const std::string &name) : Attribute(NULL, t, name)
{
	interface = i;
}

Interface *InterfaceAttribute::getInterface(void)
{
	return interface;
}
