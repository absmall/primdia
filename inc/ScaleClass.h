#ifndef __SCALECLASS_H__
#define __SCALECLASS_H__

#include <config/common.h>

#include <Primitive.h>

class ScaleClass : public Primitive
{
public:
	ScaleClass();
	void initialize();
	virtual ~ScaleClass()=0;
};

Value *solveSE(const Node *, const std::string &name);
Value *solveSC(const Node *, const std::string &name);
Value *solveCE(const Node *, const std::string &name);
Value *solve3P(const Node *, const std::string &name);

#endif /* __SCALECLASS_H__ */
