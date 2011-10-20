#ifndef __LABEL_H__
#define __LABEL_H__

#include <config/common.h>

#include <Value.h>

class Label : public Value
{
public:
	Label();
	Label(const std::string &s);
	~Label();
	const char *getText() const;
	bool operator!=(const Value &v) const;
	Value *copy() const;

	/**
	 * Return the value as a string
	 */
	std::string getString(void) const;

private:
	const char *text;
};

#endif /* __LABEL_H__ */
