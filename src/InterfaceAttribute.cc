#include <InterfaceAttribute.h>

InterfaceAttribute::InterfaceAttribute(Document *doc, Interface *i, const Type *t, const std::string &name) : Attribute(doc, NULL, t, name)
{
	interface = i;
}

Interface *InterfaceAttribute::getInterface(void)
{
	return interface;
}
