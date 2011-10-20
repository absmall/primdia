#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

/**
 * An object which holds a single piece of data that can be input by the user.
 * These are what participate in bindings.
 */

#include <config/common.h>

#include <list>
#include <string>

#include <Primitive.h>

class Attribute
{
public:
	/**
	 * A constructor. It will construct a new attribute and automatically
	 * create a new binding for it.
	 * @param n The node to which the attribute belongs
	 * @param t The type of the attribute
	 * @param name The name of the attribute
	 */
	Attribute(Node *n, const Type *t, const std::string &name);

	/**
	 * A destructor. This will release the attribute from its binding.
	 */
	~Attribute(void);

	/**
	 * Retrieve the binding associated with an attribute. An attribute
	 * is always associated with exactly one binding.
	 * @return A pointer to the binding
	 */
	Binding *getBinding(void) const;

	/**
	 * Set the binding of an attribute. This is just an accessor method,
	 * it will not register itself with the binding. It is intended to
	 * be called only from the Binding class.
	 * @param b The binding from which this attribute should obtain its
	 *          value.
	 */
	void setBinding(Binding *b);

	/**
	 * Retrieve the type of the attribute. This is constant during the
	 * lifetime of the attribute
	 * @return The attribute's type
	 */
	const Type *getType(void) const;

	/**
	 * Retrieve the node of an attribute. This may be NULL if the
	 * attribute is owned by an interface instead of a node. It
	 * is constant during the lifetime of the attribute.
	 * @return The node that owns the attribute
	 */
	Node *getNode(void) const;

	/**
	 * Determine whether the attribute has a value set by the user
	 * @return true if the attribute's value was set by the user,
	 *         false otherwise
	 */
	bool hasSetValue(void) const;

	/**
	 * Determine whether the node has a value, or is unset
	 * @return false if the attribute's value is NULL
	 *         true otherwise
	 */
	bool hasValue(void) const;

	/**
	 * Retrieve the name of the attribute
	 * @return The name of the attribute
	 */
	const std::string &getName(void) const;

private:
	std::string name;
	const Type *type;
	Node *node;
	Binding *binding;
};

#endif /* __ATTRIBUTE_H__ */
