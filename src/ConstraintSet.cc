#include <list>
#include <algorithm>

#include <ConstraintSet.h>

using namespace std;

ConstraintSet::ConstraintSet()
{
}

ConstraintSet::ConstraintSet(Value *(*rv)(const Node *n, const std::string &name, const View *view))
{
	retrieveValue = rv;
}

bool ConstraintSet::operator<(const ConstraintSet &cs) const
{
	return name < cs.name;
}

ConstraintSet::Status ConstraintSet::test(ConstraintSet *cs) const
{
	// Check if this is a superset of cs, then check if
	// cs is a superset of this.
	bool this_super_cs = true;
	bool cs_super_this = true;

	foreach(i, data)
	{
		if (find(cs->data.begin(), cs->data.end(), *i) == cs->data.end())
		{
			cs_super_this = false;
			break;
		}
	}

	foreach(i, cs->data)
	{
		if (find(data.begin(), data.end(), *i) == data.end())
		{
			this_super_cs = false;
			break;
		}
	}

	if (this_super_cs)
	{
		if (cs_super_this)
		{
			return Constrained;
		} else {
			return Underconstrained;
		}
	} else {
		return Overconstrained;
	}
}

void ConstraintSet::addConstraint(const std::string &a)
{
	name += a;
	data.push_back(a);
}
