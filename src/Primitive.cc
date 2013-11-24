#include <string>
#include <iostream>

#include <Primitive.h>
#include <ConstraintSet.h>

using namespace std;

static map<const std::string, Primitive *> &primitives()
{
	static map<const string, Primitive *> primitiveI;
	return primitiveI;
}

Primitive::Primitive(const std::string name)
{
	this->name = name;
	primitives()[name] = this;
}

Primitive::~Primitive()
{
}

void Primitive::addAttribute(const std::string &name, const Type *t)
{
	attributes.insert(pair<const std::string, const Type *>(name, t));
}

ConstraintSet &Primitive::createConstraintSubset(Value *(*completer)(const Node *, const std::string &, const View *))
{
	completions.push_back(ConstraintSet(completer));
	return completions.back();
}

Primitive *Primitive::getType(const std::string &name)
{
	return primitives()[name];
}

Primitive::TypeIterator Primitive::getAttributeStart(void)
{
	return attributes.begin();
}

Primitive::TypeIterator Primitive::getAttributeEnd(void)
{
	return attributes.end();
}

Primitive::ConstraintIterator Primitive::getConstraintStart(void) const
{
	return completions.begin();
}

Primitive::ConstraintIterator Primitive::getConstraintEnd(void) const
{
	return completions.end();
}

void Primitive::InitializeAll(void)
{
	foreach(i, primitives())
	{
		try {
			i->second->initialize();
		} catch(string message) {
			throw string("Failed to initialize ") + i->second->name + string(": ") + message;
		}
	}
}
