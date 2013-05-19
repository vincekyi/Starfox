#include "Sphere.h"

Sphere::Sphere(GLuint program, int numSubdivisions, vec4 color) {
	m_numVertices = 3 * pow(4, numSubdivisions + 1);
	m_vertexIndex = 0;
	m_color = color;
	m_program = program;

	m_vertices = new vec4[m_numVertices];
	m_normals = new vec3[m_numVertices];
	generateSphere(numSubdivisions);
}

void Sphere::generateSphere(int numSubdivisions) {
	vec4 seed[4] = {
		vec4(  0.0, 0.0, 1.0, 1.0 ),
		vec4(  0.0, 0.942809, -0.333333, 1.0 ),
		vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
		vec4(  0.816497, -0.471405, -0.333333, 1.0 )
	};

	triforce(seed[0], seed[1], seed[2], numSubdivisions);
	triforce(seed[3], seed[2], seed[1], numSubdivisions);
	triforce(seed[0], seed[3], seed[1], numSubdivisions);
	triforce(seed[0], seed[2], seed[3], numSubdivisions);

}

vec4 Sphere::normalize(const vec4& vector) {
	vec3 v = vec3(vector.x, vector.y, vector.z);
	v = Angel::normalize(v);
	vec4 result = vec4(v.x, v.y, v.z, 1.0);
	return result;
}

void Sphere::triforce(const vec4& a, const vec4& b, const vec4& c, int n) {
	vec4 v1, v2, v3;
	if (n > 0) {
		v1 = normalize(a + b);
		v2 = normalize(a + c);
		v3 = normalize(b + c);
		triforce(a, v2, v1, n - 1);
		triforce(c, v3, v2, n - 1);
		triforce(b, v1, v3, n - 1);
		triforce(v1, v2, v3, n - 1);
	}
	else {
		vec3 faceNormal = Angel::normalize(cross((c - a), (b - a)));
		if (m_shading == FLAT) {
			m_vertices[m_vertexIndex] = a;
			m_normals[m_vertexIndex] = faceNormal;
			m_vertexIndex++;
			m_vertices[m_vertexIndex] = b;
			m_normals[m_vertexIndex] = faceNormal;
			m_vertexIndex++;
			m_vertices[m_vertexIndex] = c;
			m_normals[m_vertexIndex] = faceNormal;
			m_vertexIndex++;
		} else {
			m_vertices[m_vertexIndex] = a;
			m_normals[m_vertexIndex] = vec3(a.x, a.y, a.z);
			m_vertexIndex++;
			m_vertices[m_vertexIndex] = b;
			m_normals[m_vertexIndex] = vec3(b.x, b.y, b.z);
			m_vertexIndex++;
			m_vertices[m_vertexIndex] = c;
			m_normals[m_vertexIndex] = vec3(c.x, c.y, c.z);
			m_vertexIndex++;
		}
	}
}
