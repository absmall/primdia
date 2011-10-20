#ifndef __TEXTTYPE_H__
#define __TEXTTYPE_H__

#include <config/common.h>

#include <TextClass.h>

class TextType : public TextClass
{
public:
	void render(View *canvas, const Node *n) const;

	bool boundingBox(View *canvas,
	                 const Node *n,
	                 double *left,
	                 double *right,
	                 double *top,
	                 double *bottom);
};

#endif /* __TEXTTYPE_H__ */
