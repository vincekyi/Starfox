#include "BoundingBox.h"

BoundingBox::BoundingBox() : BoundingShape(vec3(0.0f)) {
	m_halfWidths[0] = 0.0f;
	m_halfWidths[1] = 0.0f;
	m_halfWidths[2] = 0.0f;
}

BoundingBox::BoundingBox(vec3 center, float widthX, float widthY, float widthZ) : BoundingShape(center) {
	m_halfWidths[0] = widthX;
	m_halfWidths[1] = widthY;
	m_halfWidths[2] = widthZ;
}

void BoundingBox::setHalfWidths(float x, float y, float z) {
	m_halfWidths[0] = x;
	m_halfWidths[1] = y;
	m_halfWidths[2] = z; 
}

bool BoundingBox::checkCollision(BoundingShape* a) {
	return a->checkCollision(m_halfWidths, m_center);
}

bool BoundingBox::checkCollision(float* halfWidths, vec3 center) {
    if (abs(m_center.x - center.x) > (m_halfWidths[0] + halfWidths[0])) return false;
    if (abs(m_center.y - center.y) > (m_halfWidths[1] + halfWidths[1])) return false;
    if (abs(m_center.z - center.z) > (m_halfWidths[2] + halfWidths[2])) return false;
    return true;
}

bool BoundingBox::checkCollision(float radius, vec3 center) {
	float distance = radius;

	// Two extreme corners of the cube
	vec3 c1 = m_center + vec3(-m_halfWidths[0], -m_halfWidths[1], m_halfWidths[2]);
	vec3 c2 = m_center - vec3(-m_halfWidths[0], -m_halfWidths[1], m_halfWidths[2]);

	if (center.x < c1.x) distance -= abs(center.x - c1.x);
    else if (center.x > c2.x) distance -= abs(center.x - c2.x);
    if (center.y < c1.y) distance -= abs(center.y - c1.y);
    else if (center.y > c2.y) distance -= abs(center.y - c2.y);
    if (center.z < c1.z) distance -= abs(center.z - c1.z);
    else if (center.z > c2.z) distance -= abs(center.z - c2.z);
    return distance > 0;
}