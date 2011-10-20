#ifndef __LINETYPE_H__
#define __LINETYPE_H__

#include <config/common.h>

#include <LineClass.h>

class LineType : public LineClass
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

#endif /* __LINETYPE_H__ */
