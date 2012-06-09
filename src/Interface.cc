#include <map>
#include <sstream>
#include <algorithm>

#include <Node.h>
#include <Tool.h>
#include <Type.h>
#include <Binding.h>
#include <Document.h>
#include <Interface.h>
#include <Attribute.h>
#include <InterfaceAttribute.h>

using namespace std;

Interface::Interface(Document *doc, Interface *parent, std::string name, const Tool *type)
{
	this->parent = parent;
	this->name = name;
	this->type = type;
	this->doc = doc;

	map<std::string, Node *> new_nodes;

	foreach(i, type->nodes)
	{
		Node *n = new Node(doc, i->type);
		nodes.insert(n);
		new_nodes[i->name] = n;
	}

	foreach(i, type->interface)
	{
		attributes[i->name] = new InterfaceAttribute(this, Type::getType(i->type), i->name);
	}

	foreach(i, type->bindings)
	{
		Binding *b = new Binding();
		InterfaceAttribute *interface = NULL;
		foreach(j, i->attributes)
		{
			if (j->first == std::string("Interface"))
			{
				interface = attributes[j->second];
				b->mergeAttribute(interface);
			} else {
				b->mergeAttribute(new_nodes[j->first]->getAttribute(j->second));
			}
		}

		if (interface)
		{
			foreach(j, i->attributes)
			{
				if (j->first != std::string("Interface"))
				{
					interface->attributes.insert(new_nodes[j->first]->getAttribute(j->second));
				}
			}
		}

		if (i->value != std::string(""))
		{
			b->setValue(doc, Type::getType(i->type)->parse(i->value));
		}
		bindings.insert(b);
	}
}

Interface::~Interface()
{
	foreach(i, nodes)
	{
		delete *i;
	}

	doc->unregisterNode(this);
}

bool Interface::isConstrained()
{
	foreach(i, nodes)
	{
		if (!(*i)->isConstrained())
		{
			return false;
		}
	}

	return false;
}

void Interface::render(View *v)
{
	foreach(i, nodes)
	{
		(*i)->render(v);
	}
}

bool Interface::boundingBox(View *canvas,
		double *left, double *right, double *top, double *bottom)
{
	bool exists = false;
	double lm, rm, tm, bm;
	foreach(i, nodes)
	{
		double l, r, t, b;
		if ((*i)->type->boundingBox(canvas, *i, &l, &r, &t, &b))
		{
			if (exists)
			{
				lm = min(l, lm);
				rm = max(r, rm);
				bm = min(b, bm);
				tm = max(t, tm);
			} else {
				lm = l;
				rm = r;
				bm = b;
				tm = t;
				exists = true;
			}
		}
	}

	*left = lm;
	*right = rm;
	*top = tm;
	*bottom = bm;

	return exists;
}


const std::string &Interface::getName()
{
	return name;
}

void Interface::getBindings(std::set<Binding *> &bindings)
{
	foreach(i, bindings)
	{
		if (bindings.find(*i) == bindings.end())
		{
			bindings.insert(*i);
			(*i)->getBindings(bindings);
		}
	}

	foreach(i, nodes)
	{
		(*i)->getBindings(bindings);
	}
}

const Tool *Interface::getType() const
{
    return type;
}
