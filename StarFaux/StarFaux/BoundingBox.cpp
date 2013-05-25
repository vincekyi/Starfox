#include "BoundingBox.h"

BoundingBox::BoundingBox(vec3 center, float widthX, float widthY, float widthZ) {
	m_center = center;
	m_halfWidths[0] = widthX;
	m_halfWidths[1] = widthY;
	m_halfWidths[2] = widthZ;
}

void BoundingBox::setCenter(vec3 center) {
	m_center = center;
}

void BoundingBox::setHalfWidths(float x, float y, float z) {
	m_halfWidths[0] = x;
	m_halfWidths[1] = y;
	m_halfWidths[2] = z; 
}

bool BoundingBox::checkCollision(BoundingBox b) {
    if (abs(m_center.x - b.m_center.x) > (m_halfWidths[0] + b.m_halfWidths[0])) return false;
    if (abs(m_center.y - b.m_center.y) > (m_halfWidths[1] + b.m_halfWidths[1])) return false;
    if (abs(m_center.z - b.m_center.z) > (m_halfWidths[2] + b.m_halfWidths[2])) return false;
    return true;
}