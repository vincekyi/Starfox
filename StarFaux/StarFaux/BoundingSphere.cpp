#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() : BoundingShape(vec3(0.0f)) {
	m_radius = 0.0f;
}

BoundingSphere::BoundingSphere(vec3 center, float r) : BoundingShape(center) {
	m_radius = r;
}

bool BoundingSphere::checkCollision(BoundingShape* a) {
	return a->checkCollision(m_radius, m_center);
}

bool BoundingSphere::checkCollision(float* halfWidths, vec3 center) {
	float distance = pow(m_radius, 2);
	// Two extreme corners of the cube
	vec3 c1 = center - vec3(halfWidths[0], halfWidths[1], halfWidths[2]);
	vec3 c2 = center + vec3(halfWidths[0], halfWidths[1], halfWidths[2]);

	if (m_center.x < c1.x) distance -= pow((m_center.x - c1.x), 2);
    else if (m_center.x > c2.x) distance -= pow((m_center.x - c2.x), 2);
    if (m_center.y < c1.y) distance -= pow((m_center.y - c1.y), 2);
    else if (m_center.y > c2.y) distance -= pow((m_center.y - c2.y), 2);
    if (m_center.z < c1.z) distance -= pow((m_center.z - c1.z), 2);
    else if (m_center.z > c2.z) distance -= pow((m_center.z - c2.z), 2);
	if (distance > 0)
		std::cout << "WTF";
    return distance > 0;
}

bool BoundingSphere::checkCollision(float radius, vec3 center) {
 	if (length(m_center - center) < (radius + m_radius))
		return true;
    return false;
}