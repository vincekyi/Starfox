#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Types.h"
#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere(GLuint program, int numSubdivisions, vec4 color);

private:
	void generateSphere(int numSubdivisons);
	void triforce(const vec4& a, const vec4& b, const vec4& c, int n);
	vec4 normalize(const vec4& vector);
};

#endif
