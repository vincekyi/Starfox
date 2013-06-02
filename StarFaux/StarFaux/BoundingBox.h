#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

#include "Angel.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"

// Axis Aligned Bounding Box
class BoundingBox : public BoundingShape {
public:
	BoundingBox();
	BoundingBox(vec3 center, float widthX, float widthY, float widthZ);
	virtual bool checkCollision(float* halfWidths, vec3 center);
	virtual bool checkCollision(float radius, vec3 center);
	virtual bool checkCollision(BoundingShape* a);
	void setHalfWidths(float x, float y, float z);

	float m_halfWidths[3];
};

#endif