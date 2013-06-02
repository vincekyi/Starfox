#ifndef _BOUNDING_SHAPE_H_
#define _BOUNDING_SHAPE_H_

#include "Angel.h"

class BoundingShape {
public:
	BoundingShape(vec3 center) {m_center = center;}

	void setCenter(vec3 center) {m_center = center;}

	virtual bool checkCollision(BoundingShape* a) = 0;
	virtual bool checkCollision(float* halfWidths, vec3 center) = 0;
	virtual bool checkCollision(float radius, vec3 center) = 0;
	vec3 m_center;
};

#endif