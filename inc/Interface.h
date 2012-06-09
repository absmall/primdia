#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <config/common.h>

#include <map>
#include <set>
#include <list>

class Interface
{
public:
	Interface(Document *doc, Interface *parent, std::string name, const Tool *type);

	~Interface();

	bool isConstrained();

	void render(View *v);

	/**
	 * Calculate the bounding box of the interface
	 * @param canvas The canvas on which the object should be drawn. Relevant
	 *               for some of the objects where size depends on the pixel
	 *               alignment or other parts of the canvas
	 * @param left Returns the left coordinate of the bounding box.
	 * @param right Returns the right coordinate of the bounding box.
	 * @param top Returns the top coordinate of the bounding box.
	 * @param bottom Returns the bottom coordinate of the bounding box.
	 * @return true if there is a bounding box, false if none exists
	 */
	bool boundingBox(View *canvas,
	                 double *left,
	                 double *right,
	                 double *top,
	                 double *bottom);

	const std::string &getName();

	void getBindings(std::set<Binding *> &bindings);

	std::map<std::string, InterfaceAttribute *> attributes;

	std::set<Node *> nodes;
	std::set<Binding *> bindings;

    const Tool *getType() const;
private:
	std::string name;
	Document *doc;
	Interface *parent;
	const Tool *type;
};

#endif /* __INTERFACE__ */
