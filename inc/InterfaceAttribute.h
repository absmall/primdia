#ifndef __INTERFACE_ATTRIBUTE_H__
#define __INTERFACE_ATTRIBUTE_H__

#include <config/common.h>

#include <set>

#include <Attribute.h>

class InterfaceAttribute : public Attribute
{
	friend class Interface;
	friend class Binding;
public:
	InterfaceAttribute(Document *doc, Interface *i, const Type *, const std::string &name);

	/**
	 * Find what interface owns this attribute
	 * @return The interface
	 */
	Interface *getInterface(void);

private:
	/**
	 * Keep a list of hidden attributes bound to this one.
	 * When we move this one, move all of them too.
	 */
	std::set<Attribute *> attributes;

	Interface *interface;
};

#endif /* __INTERFACE_ATTRIBUTE_H__ */
