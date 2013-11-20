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

Binding::Binding(Document *document)
{
	type = NULL;
	derived = NULL;
    has_value = false;
    set_value = NULL;
    interfaceAttributes = 0;
    this->document = document;
}

Binding::Binding(Document *document, Attribute *attr)
{
	type = attr->getType();
	attr->setBinding(this);
	attributes.insert(attr);
	has_value = false;
	derived = NULL;
    set_value = NULL;
    if( attr->getNode() ) {
        interfaceAttributes = 0;
    } else {
        interfaceAttributes = 1;
    }
    this->document = document;
}

void Binding::removeAttribute(Attribute *attribute)
{
	map<Binding *, Value *> old_values;

	// Remove the binding
	attributes.erase(find(attributes.begin(), attributes.end(), attribute));

	// Make a new one
	Binding *b = new Binding(document, attribute);
	attribute->setBinding(b);

	// Transfer any dependant bindings
	if (attribute->getNode() == NULL)
	{
        interfaceAttributes --;
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
                for(map<const View *, const Value *>::const_iterator i = value.begin(); i != value.end(); i ++ ) {
                    delete i->second;
                }
				has_value = false;
			}
		}
	}

	if (derived == NULL && has_value)
	{
		// It's a set value, so copy it over
        for(map<const View *, const Value *>::iterator i = value.begin(); i != value.end(); i ++ ) {
            b->value[i->first] = i->second->copy();
        }
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
    
	if (attribute->getNode() == NULL)
    {
        interfaceAttributes --;
    }

	// If this is now empty, clear it
	if (attributes.size() == 0)
	{
		delete this;
	} else {
		propagate();
	}
}

const Value *Binding::getValue()
{
    if( set_value != NULL ) {
        return set_value;
    } else {
        return NULL;
    }
}

const Value *Binding::getValue(const View *view)
{
    if( has_value ) {
        if( set_value != NULL ) {
            return set_value;
        } else {
            return value[view];
        }
    } else {
        return NULL;
    }
}

int Binding::getInterfaceAttributes()
{
    return interfaceAttributes;
}

bool Binding::setValue(Document *doc, Value *v)
{
    bool ret;
	if (derived)
	{
		// It is always an error to assign to a binding
		// with value from elsewhere.
		return false;
	}

	if (has_value)
	{
		// We already have a value. Just swap it out and propagate from here
        foreach(i, document->getViews()) {
            delete value[*i];
            value[*i] = v->copy();
        }
        delete v;

		propagate();

        document->update(Document::SetValue, NULL, NULL, this);

		return true;
	} else {
        foreach(i, document->getViews()) {
            value[*i] = v->copy();
        }
        delete v;

		ret = propagate();

        if( ret ) {
            document->update(Document::SetValue, NULL, NULL, this);
        }

        return ret;
	}
}

bool Binding::setValue(Document *doc, Value *v, const View *view)
{
    bool ret;
	if (derived)
	{
		// It is always an error to assign to a binding
		// with value from elsewhere.
		return false;
	}

	if (has_value)
	{
		// We already have a value. Just swap it out and propagate from here
		delete value[view];
		value[view] = v;

		propagate();

        document->update(Document::SetValue, NULL, NULL, this);

		return true;
	} else {
		value[view] = v;

		ret = propagate();

        if( ret ) {
            document->update(Document::SetValue, NULL, NULL, this);
        }

        return ret;
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
	if (has_value)
	{
		// We were constrained and are no longer.
		// take note of the fact and propagate the
		// lack of value. This may underconstrain
		// some nodes.

		has_value = false;
		derived = NULL;

		propagate();
	}

	return true;
}

bool Binding::isSet(Attribute *attr) const
{
	return has_value && derived != attr;
}

bool Binding::hasValue() const
{
	return has_value;
}

bool Binding::hasSetValue() const
{
	return has_value && derived == NULL;
}

bool Binding::testValue(Attribute *setter)
{
	if (has_value)
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
	if (has_value)
	{
		if (attribute->getBinding()->has_value)
		{
			// They both have values. Fail.
			return false;
		}
	} else {
		if (attribute->getBinding()->has_value)
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
            foreach(j, (*i)->value) {
                delete j->second;
            }
			delete *i;
		}
        interfaceAttributes += oldBinding->interfaceAttributes;

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
	if (has_value && derived != attr)
	{
		// It is always an error to assign to a binding
		// with value from elsewhere.
		return false;
	}

	if (has_value)
	{
		// We already have a value, so clear it first.
        foreach(i, value) {
            delete i->second;
        }
	}

	derived = attr;
    foreach(i, value) {
        i->second = v->copy();
    }
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
	if (has_value)
	{
        foreach(i, value) {
            delete i->second;
        }
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
