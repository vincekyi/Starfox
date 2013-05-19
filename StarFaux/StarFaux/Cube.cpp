#include "Cube.h"

const int Cube::NUM_VERTICES = 36;
const vec4 Cube::CUBE_POINTS[36] = {
    vec4( -0.5f, -0.5f,  0.5f, 1.0f ),
    vec4( -0.5f,  0.5f,  0.5f, 1.0f ),
    vec4(  0.5f,  0.5f,  0.5f, 1.0f ),
    vec4(  0.5f, -0.5f,  0.5f, 1.0f ),
    vec4( -0.5f, -0.5f, -0.5f, 1.0f ),
    vec4( -0.5f,  0.5f, -0.5f, 1.0f ),
    vec4(  0.5f,  0.5f, -0.5f, 1.0f ),
    vec4(  0.5f, -0.5f, -0.5f, 1.0f )
};

Cube::Cube(GLuint program, vec4 color) {
	m_numVertices = NUM_VERTICES;
	m_vertexIndex = 0;
	m_color = color;
	m_program = program;

	m_vertices = new vec4[m_numVertices];
	m_normals = new vec3[m_numVertices];
	m_textureCoords = new vec2[m_numVertices];
	generateCube();
}

void Cube::quad(int a, int b, int c, int d, const vec3& normal) {
    m_vertices[m_vertexIndex] = CUBE_POINTS[a]; m_normals[m_vertexIndex] = normal; 
	m_textureCoords[m_vertexIndex] = vec2(0.0f, 1.0f); m_vertexIndex++;
	m_vertices[m_vertexIndex] = CUBE_POINTS[b]; m_normals[m_vertexIndex] = normal;
	m_textureCoords[m_vertexIndex] = vec2(0.0f, 0.0f); m_vertexIndex++;
	m_vertices[m_vertexIndex] = CUBE_POINTS[c]; m_normals[m_vertexIndex] = normal;
	m_textureCoords[m_vertexIndex] = vec2(1.0f, 0.0f); m_vertexIndex++;
    m_vertices[m_vertexIndex] = CUBE_POINTS[a]; m_normals[m_vertexIndex] = normal;
	m_textureCoords[m_vertexIndex] = vec2(0.0f, 1.0f); m_vertexIndex++;
    m_vertices[m_vertexIndex] = CUBE_POINTS[c]; m_normals[m_vertexIndex] = normal;
	m_textureCoords[m_vertexIndex] = vec2(1.0f, 0.0f); m_vertexIndex++;
    m_vertices[m_vertexIndex] = CUBE_POINTS[d]; m_normals[m_vertexIndex] = normal;
	m_textureCoords[m_vertexIndex] = vec2(1.0f, 1.0f); m_vertexIndex++;
}

void Cube::generateCube()
{
    quad( 1, 0, 3, 2, vec3( 0.0f,  0.0f,  1.0f) );
    quad( 2, 3, 7, 6, vec3( 1.0f,  0.0f,  0.0f) );
    quad( 3, 0, 4, 7, vec3( 0.0f, -1.0f,  0.0f) );
    quad( 6, 5, 1, 2, vec3( 0.0f,  1.0f,  0.0f) );
    quad( 6, 7, 4, 5, vec3( 0.0f,  0.0f, -1.0f) );
    quad( 5, 4, 0, 1, vec3(-1.0f,  0.0f,  0.0f) );
}