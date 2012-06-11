#ifndef __NODE_H__
#define __NODE_H__

#include <config/common.h>

#include <map>
#include <set>

#include <Primitive.h>
#include <ConstraintSet.h>

class Node
{
public:
	const Value *getValue(const std::string &s) const;

	/**
	 * Recalculate which constraint set should be used for a node.
	 * @return true if the node is not overconstrained, false otherwise
	 */
	bool update();

	/**
	 * Recalculate which constraint set should be used for a node.
	 * Include bindings in test mode
	 * @param b The bindings that have been modified so far and who
	 *          set them.
	 */
	bool testUpdate(std::map<Binding *, Attribute *> &bindings);

	ConstraintSet::Status testStatus(ConstraintSet *cs) const;

	bool isConstrained(void);

	Attribute *getAttribute(const std::string &name);

	/**
	 * Retrieves the set of Bindings in which any attribute of the node
	 * participates by inserting them all into the bindings set.
	 * @param bindings The set in which to insert all bindings
	 */
	void getBindings(std::set<Binding *> &bindings);

	const Value *calcValue(const std::string &name, const View *view) const;

	void render(View *canvas);

	Document *d;
	Primitive *type;

	std::map<const std::string, Attribute *> attributes;

private:
	friend class Attribute;
	friend class Binding;
	friend class Interface;
	Node(Document *doc, const std::string &type);

	~Node();

	static std::set<Node *> nodes;
	Primitive::ConstraintIterator current_completion;
};

#endif /* __NODE_H__ */
