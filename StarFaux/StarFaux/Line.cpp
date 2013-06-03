#include "Line.h"
#include "Shape.h"

Line::Line(GLuint program, double length) {
    m_program = program;
	m_numVertices = 2;
    m_vertices = new vec4[m_numVertices];
	m_normals = new vec3[m_numVertices];
    m_shapeType = LINE;
	m_shading = NONE;

	generateLine(length);
}

void Line::draw(Camera& camera, Light* light, lightEffects effects) {
	Shape::draw(MESH, camera, light, effects);
}

void Line::resetPosition() {
	// x: -10 to 10
	// y: -10 to 10
	// z: 0 to -50
	translate((rand() % 10) * 2.0f - 10.0f, (rand() % 10) * 2.0f - 10.0f, -(rand() % 50));
}


void Line::generateLine(double length) {
    double coord = length / 2.0;
    m_vertices[0] = vec4(0.0, 0.0, -coord, 1.0);
    m_vertices[1] = vec4(0.0, 0.0, coord, 1.0);

	// Fake normals to be lazy
	m_normals[0] = vec3(0.0, 0.0, 0.0);
	m_normals[1] = vec3(0.0, 0.0, 0.0);
}

