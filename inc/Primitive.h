#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

/**
 * This describes the class of a node. It contains virtual functions which should
 * be overridden to create a new type for nodes.
 */

#include <config/common.h>

#include <map>
#include <list>

class Primitive
{
public:
	/**
	 * Default constructor
	 * @param name The name of the new type of primitive
	 */
	Primitive(const std::string name);

	/**
	 * Initialize the set of attributes and constraints used by the primitive
	 */
	virtual void initialize()=0;

	/**
	 * This function must be overridden and is responsible for drawing any
	 * nodes of this type
	 * @param canvas The canvas on which the node should be drawn
	 * @param n The node to draw
	 */
	virtual void render(View *canvas, const Node *n) const=0;

	/**
	 * Calculate the bounding box of the node
	 * @param canvas The canvas on which the object should be drawn. Relevant
	 *               for some of the objects where size depends on the pixel
	 *               alignment or other parts of the canvas
	 * @param n The node for which the bounding box should be calculated
	 * @param left Returns the left coordinate of the bounding box.
	 * @param right Returns the right coordinate of the bounding box.
	 * @param top Returns the top coordinate of the bounding box.
	 * @param bottom Returns the bottom coordinate of the bounding box.
	 * @return true if there is a bounding box, false if none exists
	 */
	virtual bool boundingBox(View *canvas,
	                         const Node *n,
	                         double *left,
	                         double *right,
	                         double *top,
	                         double *bottom)=0;

	/**
	 * Default destructor
	 */
	virtual ~Primitive()=0;

	/**
	 * Return the primitive with some name
	 * @param name The name to search for. A primitive with this name
	 *             must exist.
	 * @return A pointer to a primitive
	 */
	static Primitive *getType(const std::string &name);

	/**
	 * Register the primitives and constraint sets used by each primitive
	 */
	static void InitializeAll(void);

	/**
	 * The list of completions that will constrain the primitive.
	 */
	std::list<ConstraintSet> completions;

	/**
	 * The name of this primitive
	 */
	std::string name;

protected:
	/**
	 * An iterate that iterates over the attributes of the primitive.
	 */
	typedef std::map<const std::string, const Type *>::iterator TypeIterator;

	/**
	 * An iterator that iterates over the constraint sets of the primitive.
	 */
	typedef std::list<ConstraintSet>::const_iterator ConstraintIterator;

	/**
	 * Add an attribute to this primitives attribute list
	 * @param name The name of the attribute
	 * @param t The type of the attribute
	 */
	void addAttribute(const std::string &name, const Type *t);

	/**
	 * Create a new constraining subset for this primitives
	 * @param completer The completion function that will be used for calculating
	 *                  unset attributes once the node is constrained
	 */
	ConstraintSet &createConstraintSubset(Value *(*completer)(const Node *,
	                                                          const std::string &));

	/**
	 * Return an iterator to the start of the primitive's attribute list
	 * @return An iterator
	 */
	TypeIterator getAttributeStart();

	/**
	 * Return an iterator to the end of the primitive's attribute list
	 * @return An iterator
	 */
	TypeIterator getAttributeEnd();

	/**
	 * Return an iterator to the start of this primitive's list of constraining
	 * subsets
	 * @return An iterator
	 */
	ConstraintIterator getConstraintStart() const;

	/**
	 * Return an iterator to the end of this primitive's list of constraining
	 * subsets
	 * @return An iterator
	 */
	ConstraintIterator getConstraintEnd() const;

private:
	friend class Node;
	friend class Attribute;

	/**
	 * List of attributes of this primitive
	 */
	std::map<const std::string, const Type *> attributes;
};

#endif /* __PRIMITIVE_H__ */
