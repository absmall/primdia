#include <string.h>

#include <Font.h>
#include <FontType.h>

FontType::FontType() : Type("Font")
{
}

Font::Font()
{
}

Font::~Font()
{
	delete font;
}

Font::Font(const std::string &s)
{
	font = strdup(s.c_str());
}

const char *Font::getFont() const
{
	return font;
}

bool Font::operator!=(const Value &v) const
{
	const Font &f = static_cast<const Font &>(v);
	return strcmp(font, f.font);
}

Value *Font::copy() const
{
	return new Font(font);
}

std::string Font::getString(void) const
{
	return std::string(font);
}

Value *FontType::parse(const std::string &s) const
{
	return new Font(s);
}
