#ifndef __TEXTCLASS_H__
#define __TEXTCLASS_H__

#include <config/common.h>

#include <Primitive.h>

class TextClass : public Primitive
{
public:
	TextClass();
	void initialize();
	virtual ~TextClass()=0;

private:
};

Value *solveTLBR(const Node *, const std::string &name, const View *view);
Value *solveTRBL(const Node *, const std::string &name, const View *view);

#endif /* __TEXTCLASS_H__ */
