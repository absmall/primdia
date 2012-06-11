#ifndef __POINTCLASS_H__
#define __POINTCLASS_H__

#include <config/common.h>

#include <Primitive.h>

class PointClass : public Primitive
{
public:
	PointClass();
	void initialize();
	virtual ~PointClass()=0;

private:
};

Value *solveP(const Node *, const std::string &name, const View *view);

#endif /* __POINTCLASS_H__ */
