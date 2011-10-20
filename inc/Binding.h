#ifndef __BINDING_H__
#define __BINDING_H__

/**
 * A binding is over a list of attributes. These attributes share a common value.
 * The sets of attributes are necessarily disjoint since if one element of a second
 * binding was a member of a first, they must have a common value, and all those in
 * each binding must have a common value, meaning they'd really all be one binding.
 */
#include <config/common.h>

#include <map>
#include <set>
#include <list>

class Binding
{
public:
	// User functions. These should be accessed by code invoked by the user

	/**
	 * Construct a new empty binding. This should only be used for constructing
	 * Interaces
	 */
	Binding(void);

	/**
	 * Create a new binding containing a single attribute. Every attribute must
	 * participate in a binding at all times, so this is the standard method for
	 * creating a binding for a new attribute.
	 * @param attr The attribute about which the binding will be constructed
	 */
	Binding(Attribute *attr);

	/**
	 * Remove an attribute from a binding. It and any hidden dependants will
	 * be placed in a new binding. It the previous binding had a value, it
	 * will be copied here.
	 * @param attr The attribute to remove from this binding
	 */
	void removeAttribute(Attribute *attr);

	/**
	 * Remove an attribute from a binding. Since this ignores dependents, it
	 * is probably only useful from an attribute destructor.
	 * @param attr The attribute to release from the binding
	 */
	void releaseAttribute(Attribute *attr);

	/**
	 * Retrieve the value from a binding.
	 * @return The value of the binding. May be NULL.
	 */
	const Value *getValue(void);

	/**
	 * Set the value from a binding
	 * @param doc The document of the value. Callbacks will be passed there
	 * @param v The new value to set
	 * @return true if the value could be set, false otherwise
	 */
	bool setValue(Document *doc, Value *v);

	/**
	 * Remove the value from a binding.
	 * @param doc The document of the attribute. Callbacks will be passed there
	 * @param attr Permission to remove the value will be denied if the
	 *             value was not initially set by attr
	 * @return true if the value could be unset, false otherwise.
	 */
	bool unsetValue(Document *doc, Attribute *attr);

	/**
	 * Determine whether a binding has an initialized value
	 * @param attr The value doesn't count if it was derived from attr
	 * @return true if the value was derived from any value besides attr,
	 *         or it was set by the user.
	 */
	bool isSet(Attribute *attr) const;

	/**
	 * Determine whether a bindings has any value
	 * @return true if the value of the bindings is not NULL, false otherwise.
	 */
	bool hasValue() const;

	/**
	 * Determine whether a binding has a value set by the user
	 * @return true if the bindings has a value and it was set by the user,
	 *         false otherwise.
	 */
	bool hasSetValue() const;

	/**
	 * Determine whether the binding could be set without
	 * overconstraining anything in the diagram.
	 * @param setter Who is setting this value. Default to no one.
	 * @return true if the value can be set without causing an overconstraint,
	 *         false if it would cause an overconstraint.
	 */
	bool testValue(Attribute *setter = NULL);

	/**
	 * Merge the attributes bound to a particular attribute into another
	 * binding.
	 * @param attr All attributes bound to attr will be inserted into this binding
	 * @return true if the attributes of the two bindings could be merged into
	 *         a single binding, false otherwise.
	 */
	bool mergeAttribute(Attribute *attr);

	// System functions. These should be accessed by code indirectly invoked by
	// binding code

	/**
	 * Set a value in a binding. This should only be used to set derived values,
	 * meaning it should probably be used only by the Node class when constraint
	 * rules are being applied.
	 * @param v The value to set in the binding
	 * @param attr The attribute which will be listed as the one from which the
	 *             value is derived. Used for recalculating values.
	 */
	bool setValue(Value *v, Attribute *attr);

	/**
	 * Get all bindings that can be reached from this one by searching other
	 * attributes of nodes
	 * @param bindings a place where the bindings should be recorded.
	 */
	void getBindings(std::set<Binding *> &bindings);
	
	/**
	 * The type of the binding. This is a cache since it's equal to the
	 * value of all participating attributes.
	 */
	const Type *type;

	/**
	 * Each binding is between a set of attributes from possibly
	 * different primitives.
	 */
	std::set<Attribute *> attributes;

private:
	/**
	 * Default destructor
	 */
	~Binding();

	/**
	 * Propagate existence of a value through bindings. This is used
	 * to test whether an overconstraint would result when a binding's
	 * value is set without actually setting the value.
	 * @param b The set of bindings modified so far and who set them.
	 * @param setter Who is setting this node
	 * @return true if the value can be propagated without causing
	 *         overconstraints, false otherwise.
	 */
	bool testPropagate(std::map<Binding *, Attribute *> &b, Attribute *setter);

	/**
	 * Propagate the value from a binding to fill all
	 * bindings that are fully constrained by it. This
	 * should only be used when it is known that the
	 * graph is valid.
	 * @return true if propagation can occur, false otherwise 
	 */
	bool propagate();

	/**
	 * Indicates whether the value is independent of the view
	 */
	bool uses_view;

	/**
	 * The common value of the attributes of this binding.
	 * It may be NULL
	 */
	const Value *value;

	/**
	 * Indicates whether the value for this binding was set
	 * by the user, or derived from an edge
	 */
	Attribute *derived;
};

#endif /* __BINDING_H__ */
