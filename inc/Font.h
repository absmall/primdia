#ifndef __FONT_H__
#define __FONT_H__

#include <config/common.h>

#include <Value.h>

class Font : public Value
{
public:
	Font();
	Font(const std::string &s);
	~Font();
	const char *getFont() const;
	bool operator!=(const Value &v) const;
	Value *copy() const;

	/**
	 * Return the value as a string
	 */
	std::string getString(void) const;

private:
	const char *font;
};

#endif /* __FONT_H__ */
