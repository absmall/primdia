#include <list>
#include <queue>
#include <algorithm>

#include <Node.h>
#include <View.h>
#include <Value.h>
#include <Binding.h>
#include <Document.h>
#include <Attribute.h>
#include <ConstraintSet.h>
#include <InterfaceAttribute.h>

using namespace std;

Binding::Binding(void)
{
	type = NULL;
	derived = NULL;
	value = NULL;
}

Binding::Binding(Attribute *attr)
{
	type = attr->getType();
	attr->setBinding(this);
	attributes.insert(attr);
	value = NULL;
	derived = false;
}

void Binding::removeAttribute(Attribute *attribute)
{
	map<Binding *, Value *> old_values;

	// Remove the binding
	attributes.erase(find(attributes.begin(), attributes.end(), attribute));

	// Make a new one
	Binding *b = new Binding(attribute);
	attribute->setBinding(b);

	// Transfer any dependant bindings
	if (attribute->getNode() == NULL)
	{
		InterfaceAttribute *ia = static_cast<InterfaceAttribute *>(attribute);
		foreach(i, ia->attributes)
		{
			attributes.erase(find(attributes.begin(), attributes.end(), *i));
			b->attributes.insert(*i);
			(*i)->setBinding(b);

			// If this attribute was setting a binding, release the value
			if (derived == *i)
			{
				derived = NULL;
				delete value;
				value = NULL;
			}
		}
	}

	if (derived == NULL && value != NULL)
	{
		// It's a set value, so copy it over
		b->value = value->copy();
	}

	propagate();
	b->propagate();

	// If this is now empty, clear it
	if (attributes.size() == 0)
	{
		delete this;
	}
}

void Binding::releaseAttribute(Attribute *attribute)
{
	// Remove the binding
	attributes.erase(find(attributes.begin(), attributes.end(), attribute));

	if (attribute == derived)
	{
		derived = NULL;
	}

	// If this is now empty, clear it
	if (attributes.size() == 0)
	{
		delete this;
	} else {
		propagate();
	}
}

const Value *Binding::getValue(void)
{
	return value;
}

bool Binding::setValue(Document *doc, Value *v)
{
	if (derived)
	{
		// It is always an error to assign to a binding
		// with value from elsewhere.
		return false;
	}

	if (value)
	{
		// We already have a value. Just swap it out and propagate from here
		delete value;
		value = v;

		propagate();

		return true;
	} else {
		value = v;

		return propagate();
	}
}

bool Binding::unsetValue(Document *doc, Attribute *attr)
{
	// If this guy didn't set the value, don't unset it
	if (derived != attr)
	{
		// It is always an error to assign to a binding
		// with derived value.
		return false;
	}

	// It's always safe to unconstrain a binding,
	// since that can never cause an overconstraint
	// situation.
	if (value != NULL)
	{
		// We were constrained and are no longer.
		// take note of the fact and propagate the
		// lack of value. This may underconstrain
		// some nodes.

		value = NULL;
		derived = NULL;

		propagate();
	}

	return true;
}

bool Binding::isSet(Attribute *attr) const
{
	return value != NULL && derived != attr;
}

bool Binding::hasValue() const
{
	return value != NULL;
}

bool Binding::hasSetValue() const
{
	return value != NULL && derived == NULL;
}

bool Binding::testValue(Attribute *setter)
{
	if (value)
	{
		// Can never set something that already has a value
		return false;
	}

	map<Binding *, Attribute *> bindings_modified;
	return testPropagate(bindings_modified, setter);
}

bool Binding::mergeAttribute(Attribute *attribute)
{
	Binding *oldBinding;

	// Make sure types match
	if (type == NULL)
	{
		type = attribute->getType();
	} else {
		if (type != attribute->getType())
		{
			return false;
		}
	}

	// Make sure they don't both have values
	if (value != NULL)
	{
		if (attribute->getBinding()->value != NULL)
		{
			// They both have values. Fail.
			return false;
		}
	} else {
		if (attribute->getBinding()->value != NULL)
		{
			// The other one's got a value, use it
			value = attribute->getBinding()->value;
			derived = attribute->getBinding()->derived;
		}
	}

	set<Binding *> old_bindings;

	// Keep the old binding so we can back out
	oldBinding = attribute->getBinding();
	old_bindings.insert(oldBinding);

	// Set the new binding
	foreach(i, oldBinding->attributes)
	{
		(*i)->setBinding(this);
		attributes.insert(*i);
	}

	// See if it succeeded
	if (propagate())
	{
		foreach(i, old_bindings)
		{
			(*i)->value = NULL;
			delete *i;
		}

		return true;
	} else {
		foreach(i, old_bindings)
		{
			foreach(j, (*i)->attributes)
			{
				(*j)->setBinding(*i);
			}
		}

		return false;
	}
}

bool Binding::setValue(Value *v, Attribute *attr)
{
	if (value != NULL && derived != attr)
	{
		// It is always an error to assign to a binding
		// with value from elsewhere.
		return false;
	}

	if (value)
	{
		// We already have a value, so clear it first.
		delete value;
	}

	derived = attr;
	value = v;
	if( propagate()) {
		return true;
	} else {
		return false;
	}
}

void Binding::getBindings(std::set<Binding *> &bindings)
{
	foreach(i, attributes)
	{
		if ((*i)->getNode() != NULL)
		{
			(*i)->getNode()->getBindings(bindings);
		}
	}
}

Binding::~Binding(void)
{
	if (value != NULL)
	{
		delete value;
	}
}

bool Binding::testPropagate(std::map<Binding *, Attribute *> &b, Attribute *setter)
{
	if (b.find(this) != b.end())
	{
		// We've been touched already. Technically this
		// is a cycle, though as long as the value that
		// is attempted to be set is equal, it's not a
		// problem.
		return true;
	}

	b[this] = setter;

	foreach(i, attributes)
	{
		if ((*i)->getNode()
		&& !(*i)->getNode()->testUpdate(b))
		{
			return false;
		}
	}

	return true;
}


bool Binding::propagate()
{
	// See if this binding fully constrains any nodes.
	// If so, propagate recursively on all of their unspecified values
	foreach(i, attributes)
	{
		if ((*i)->getNode() && !(*i)->getNode()->update())
		{
			return false;
		}
	}

	return true;
}
