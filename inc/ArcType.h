#ifndef __ARCTYPE_H__
#define __ARCTYPE_H__

#include <config/common.h>

#include <ArcClass.h>

class ArcType : public ArcClass
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

#endif /* __ARCTYPE_H__ */
