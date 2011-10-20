#ifndef __ARCCLASS_H__
#define __ARCCLASS_H__

#include <config/common.h>

#include <Primitive.h>

class ArcClass : public Primitive
{
public:
	ArcClass();
	void initialize();
	virtual ~ArcClass()=0;

private:
};

Value *solveA(const Node *, const std::string &name);

#endif /* __ARCCLASS_H__ */
