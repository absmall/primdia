#ifndef __GTKFONTTYPE_H__
#define __GTKFONTTYPE_H__

#include <config/common.h>

#include <FontType.h>

/**
 * The type for font descriptors.
 */

class GtkFontType : public FontType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
};

#endif /* __GTKFONTTYPE_H__ */
