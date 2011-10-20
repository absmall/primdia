#include <Node.h>
#include <Value.h>
#include <Binding.h>
#include <Document.h>
#include <Attribute.h>
#include <Primitive.h>
#include <ConstraintSet.h>
#include <algorithm>

using namespace std;

Node::Node(Document *doc, const std::string &s)
{
	// Set the type
	type = Primitive::getType(s);

	// Construct the attributes
	for(Primitive::TypeIterator iter =  type->getAttributeStart();
	                            iter != type->getAttributeEnd();
	                            iter ++)
	{
		attributes[iter->first] = new Attribute(this, iter->second, iter->first);
	}

	// Set whether the node is fully constrained
	current_completion = type->getConstraintEnd();

	d = doc;
}

Node::~Node()
{
	foreach(i, attributes)
	{
		delete i->second;
	}

	attributes.clear();
}

void Node::render(View *canvas)
{
	if (current_completion != type->getConstraintEnd())
	{
		type->render(canvas, this);
	}
}

const Value *Node::getValue(const std::string &s) const
{
	const Attribute *a = attributes.find(s)->second;
	Binding *b = a->getBinding();
	return b->getValue();
}

bool Node::isConstrained(void)
{
	return current_completion == type->getConstraintEnd();
}

Attribute *Node::getAttribute(const std::string &name)
{
	map<const std::string, Attribute *>::iterator i = attributes.find(name);
	if (i != attributes.end())
	{
		return i->second;
	} else {
		throw std::string("Invalid attribute: ") + name;
	}
}

void Node::getBindings(std::set<Binding *> &bindings)
{
	foreach(i, attributes)
	{
		if (bindings.find(i->second->getBinding()) == bindings.end())
		{
			bindings.insert(i->second->getBinding());
			i->second->getBinding()->getBindings(bindings);
		}
	}
}

const Value *Node::calcValue(const std::string &name) const
{
	return current_completion->retrieveValue(this, name);
}

const Value *Node::calcValue(const std::string &name, const View *view) const
{
	return current_completion->retrieveValueView(this, name, view);
}

bool Node::testUpdate(std::map<Binding *, Attribute *> &bindings)
{
	ConstraintSet cs;

	// Build ConstraintSet for the attributes of this node
	// that have been set, or are in test mode
	foreach(i, attributes)
	{
		Binding *b = i->second->getBinding();
		std::map<Binding *, Attribute *>::iterator j = bindings.find(b);

		// There are two ways that a value can be set: it can have
		// real data, or it can be in the test set as something
		// not derived from here
		if (((j != bindings.end())  && (j->second != i->second))
		|| (b->isSet(i->second)))
		{
			cs.addConstraint(i->first);
		}
	}


	Primitive::ConstraintIterator completion = type->getConstraintEnd();
	bool has_underconstrained_solution = false;

	for(Primitive::ConstraintIterator i = type->getConstraintStart();
		i != type->getConstraintEnd();
		i++)
	{
		ConstraintSet::Status s = i->test(&cs);
		if (ConstraintSet::Constrained == s)
		{
			// Found someone!
			completion = i;
			has_underconstrained_solution = true;
			break;
		} else if (ConstraintSet::Underconstrained == s) {
			has_underconstrained_solution = true;
		}
	}

	if (!has_underconstrained_solution)
	{
		// We can't do anything with this node.
		return false;
	}

	if (completion != type->getConstraintEnd())
	{
		// Check the values and set them if necessary
		foreach(i, attributes)
		{
			if (find(completion->data.begin(),
				 completion->data.end(),
				 i->first) == completion->data.end())
			{
				Binding *b = i->second->getBinding();
				if (!b->testValue(i->second)) return false;
			}
		}
	}

	return true;
}

ConstraintSet::Status Node::testStatus(ConstraintSet *cs) const
{
	foreach(i, type->completions)
	{
		ConstraintSet::Status s = i->test(cs);
		switch(s)
		{
			case ConstraintSet::Underconstrained:
			{
				// Someone is fillable, so any constraining
				// set would be a superset (invalid)
				return s;
			}
			case ConstraintSet::Constrained:
			{
				// This is our match, go with it
				return s;
			}
			case ConstraintSet::Overconstrained:
			{
				// Ignore
				break;
			}
		}
	}

	return ConstraintSet::Overconstrained;
}

bool Node::update()
{
	ConstraintSet cs;

	// Build ConstraintSet for the attributes of this node that have been set
	foreach(i, attributes)
	{
		if (i->second->getBinding()->isSet(i->second))
		{
			cs.addConstraint(i->first);
		}
	}

	// Unset everything from the old completion
	Primitive::ConstraintIterator previous_completion = current_completion;
	current_completion = type->getConstraintEnd();
	bool has_underconstrained_solution = false;

	for(Primitive::ConstraintIterator i = type->getConstraintStart(); i != type->getConstraintEnd(); i++)
	{
		ConstraintSet::Status s = i->test(&cs);
		if (ConstraintSet::Constrained == s)
		{
			// Found someone!
			current_completion = i;
			has_underconstrained_solution = true;
			break;
		} else if (ConstraintSet::Underconstrained == s) {
			has_underconstrained_solution = true;
		}
	}

	// If something changed, clear everything
	if (previous_completion != current_completion
	 && previous_completion != type->getConstraintEnd())
	{
		if (current_completion == type->getConstraintEnd())
		{
			foreach(i, attributes)
			{
				i->second->getBinding()->unsetValue(d, i->second);
			}
		} else {
			foreach(i, attributes)
			{
				if (find(current_completion->data.begin(),
					 current_completion->data.end(),
					 i->first) == current_completion->data.end())
				{
					i->second->getBinding()->unsetValue(d, i->second);
				}
			}
		}
	} else if (current_completion != type->getConstraintEnd()) {
		// Check the values and set them if necessary
		foreach(i, attributes)
		{
			std::string s = i->first;
			if (find(current_completion->data.begin(),
				 current_completion->data.end(),
				 i->first) == current_completion->data.end())
			{
                Value *v = current_completion->retrieveValue(this, i->first);
                Binding *b = i->second->getBinding();
                if (b->getValue() == NULL || *b->getValue() != *v)
                {
                    i->second->getBinding()->setValue(v, i->second);
                } else {
                    delete v;
                }
			}
		}
	}

	return has_underconstrained_solution;
}
