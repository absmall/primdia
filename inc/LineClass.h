#ifndef __LINECLASS_H__
#define __LINECLASS_H__

#include <config/common.h>

#include <Primitive.h>

class LineClass : public Primitive
{
public:
	LineClass();
	void initialize();
	virtual ~LineClass()=0;

private:
};

Value *solvePP(const Node *, const std::string &name);
Value *solveP1LA(const Node *, const std::string &name);
Value *solveP2LA(const Node *, const std::string &name);

#endif /* __LINECLASS_H__ */
