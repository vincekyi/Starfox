#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#include "Angel.h"
#include "BoundingShape.h"
#include "BoundingBox.h"

class BoundingSphere : public BoundingShape {
public:
	BoundingSphere();
	BoundingSphere(vec3 center, float r);
	virtual bool checkCollision(float* halfWidths, vec3 center);
	virtual bool checkCollision(float radius, vec3 center);
	virtual bool checkCollision(BoundingShape* a);
	
	float m_radius;
};

#endif