#ifndef __CONSTRAINTSET_H__
#define __CONSTRAINTSET_H__

#include <config/common.h>

#include <list>
#include <string>

class ConstraintSet
{
public:
	enum Status
	{
		Underconstrained,
		Constrained,
		Overconstrained
	};

	ConstraintSet();
	ConstraintSet(Value *(*)(const Node *, const std::string &name, const View *view));
	std::string name;
	std::list<std::string> data;
	void addConstraint(const std::string &a);
	Status test(ConstraintSet *cs) const;
	bool uses_view;

	// This would be better as a member function pointer, but cannot be
	// until gcc bug 8171 is resolved.
	Value *(*retrieveValue)(const Node *, const std::string &name, const View *vew);
	bool operator<(const ConstraintSet &) const;
};


#endif /* __CONSTRAINTSET_H__ */
