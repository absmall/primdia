#include <Attribute.h>
#include <Node.h>
#include <Binding.h>

Attribute::Attribute(Document *doc, Node *n, const Type *t, const std::string &name)
{
	node = n;
	type = t;
	binding = new Binding(doc, this);
	this->name = name;
}

Attribute::~Attribute()
{
	binding->releaseAttribute(this);
}

Binding *Attribute::getBinding() const
{
	return binding;
}

void Attribute::setBinding(Binding *b)
{
	// If this has no primitive and it isn't currently in use, it doesn't need to be preserved
	if (node == NULL && b == NULL )
	{
		delete this;
	}

	binding = b;
}

const Type *Attribute::getType(void) const
{
	return type;
}

Node *Attribute::getNode(void) const
{
	return node;
}

bool Attribute::hasSetValue(void) const
{
	return binding->hasSetValue();
}

bool Attribute::hasValue(void) const
{
	return binding->hasValue();
}

const std::string &Attribute::getName(void) const
{
	return name;
}
