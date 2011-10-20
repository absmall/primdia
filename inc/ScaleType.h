#ifndef __SCALETYPE_H__
#define __SCALETYPE_H__

#include <config/common.h>

#include <ScaleClass.h>

class ScaleType : public ScaleClass
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

#endif /* __SCALETYPE_H__ */
