#ifndef __VALUE_H__
#define __VALUE_H__

#include <config/common.h>

class Value
{
public:
	Value();
	virtual ~Value()=0;
	virtual bool operator!=(const Value &v) const=0;
	virtual Value *copy() const=0;
	virtual std::string getString(void) const=0;
};

#endif /* __VALUE_H__ */
