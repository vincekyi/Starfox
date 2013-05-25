#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

#include "Angel.h"

// Axis Aligned Bounding Box
class BoundingBox {
public:
	BoundingBox(vec3 center, float widthX, float widthY, float widthZ);
	vec3 m_center;
	float m_halfWidths[3];
	bool checkCollision(BoundingBox b);
	void setCenter(vec3 center);
	void setHalfWidths(float x, float y, float z);
};

#endif