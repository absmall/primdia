#ifndef __POINTTYPE_H__
#define __POINTTYPE_H__

#include <config/common.h>

#include <PointClass.h>

class PointType : public PointClass
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

#endif /* __POINTTYPE_H__ */
