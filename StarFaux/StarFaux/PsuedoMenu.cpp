#include "PsuedoMenu.h"

const vec4 PsuedoMenu::squarePoints[NUMPOINTS] = {
    vec4( -0.5f, -0.5f, -0.5f, 1.0f ),
	vec4(  0.5f,  0.5f, -0.5f, 1.0f ),
    vec4( -0.5f,  0.5f, -0.5f, 1.0f ),
    
	vec4( -0.5f, -0.5f, -0.5f, 1.0f ),
	vec4(  0.5f, -0.5f, -0.5f, 1.0f ),
	vec4(  0.5f,  0.5f, -0.5f, 1.0f ),
};

PsuedoMenu::PsuedoMenu(GLuint program, ShadingType shading, float height, float width) {
	m_program = program;
	m_shading = shading;
	m_numVertices = NUMPOINTS;
	m_shapeType = CUBE;

	generateMenu(height, width);
}

void PsuedoMenu::generateMenu(float height, float width) {
	m_vertices = (vec4*)malloc(sizeof(vec4) * m_numVertices);
	m_normals = (vec3*)malloc(sizeof(vec3) * m_numVertices);
	m_textureCoords = (vec2*)malloc(sizeof(vec2) * m_numVertices);
	for (int i = 0; i < m_numVertices; i++) {
		//m_vertices[i] = squarePoints[i];
		m_normals[i] = vec3(0.0, 0.0, 1.0);
	}

	m_vertices[0] = vec4(-width/2.0, -height/2.0, -0.5, 1.0);
	m_vertices[1] = vec4( width/2.0,  height/2.0, -0.5, 1.0);
	m_vertices[2] = vec4(-width/2.0,  height/2.0, -0.5, 1.0);

	m_vertices[3] = vec4(-width/2.0, -height/2.0, -0.5, 1.0);
	m_vertices[4] = vec4( width/2.0, -height/2.0, -0.5, 1.0);
	m_vertices[5] = vec4( width/2.0,  height/2.0, -0.5, 1.0);

	m_textureCoords[0] = vec2(0.0, 0.0);
	m_textureCoords[1] = vec2(1.0, 1.0);
	m_textureCoords[2] = vec2(0.0, 1.0);

	m_textureCoords[3] = vec2(0.0, 0.0);
	m_textureCoords[4] = vec2(1.0, 0.0);
	m_textureCoords[5] = vec2(0.0, 1.0);

}
