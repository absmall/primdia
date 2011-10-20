#ifndef __FLAG_H__
#define __FLAG_H__

#include <Value.h>

class Flag : public Value
{
public:
	Flag(bool value);
	Flag(const std::string &s);
	~Flag();

	bool getValue(void) const;
	bool operator != (const Value &v) const;

	Value *copy() const;
	std::string getString(void) const;

private:
	bool status;
};

#endif /* __FLAG_H__ */
