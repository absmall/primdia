#include <string.h>

#include <Label.h>
#include <LabelType.h>

LabelType::LabelType() : Type("Text")
{
}

Label::Label()
{
}

Label::~Label()
{
	delete text;
}

Label::Label(const std::string &s)
{
	text = strdup(s.c_str());
}

const char *Label::getText() const
{
	return text;
}

bool Label::operator!=(const Value &v) const
{
	const Label &l = static_cast<const Label &>(v);
	return strcmp(text, l.text);
}

Value *Label::copy() const
{
	Label *ret = new Label();

	ret->text = strdup(text);

	return ret;
}

std::string Label::getString(void) const
{
	return std::string(text);
}

Value *LabelType::parse(const std::string &s) const
{
	return new Label(s);
}
