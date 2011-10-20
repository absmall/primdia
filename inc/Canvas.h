#ifndef __CANVAS_H__
#define __CANVAS_H__

/**
 * A canvas on which all objects will be rendered
 */
#include <config/common.h>

class Canvas
{
public:
	/**
	 * Class destructor
	 */
	virtual ~Canvas();

	/**
	 * Translate a point from pixel coordinates to canvas
	 * coordinates
	 * @param x Inputs the x value in pixel coordinates,
	 *          Outputs the x value in canvas coordinates
	 * @param y Inputs the y value in pixel coordinates,
	 *          Outputs the y value in canvas coordinates
	 */
	virtual void toCanvas(double *x, double *y)=0;
};

#endif /* __CANVAS_H__ */
