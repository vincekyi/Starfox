#ifndef _CUBE_H_
#define _CUBE_H_

#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Types.h"
#include "Shape.h"
#include <string>

class Cube : public Shape {
public:
	
	Cube(GLuint program, vec4 color,  ShadingType shading, float shininess, vec4 specularColor, bool useTexture, TextureSamplingType samplingType, TextureWrappingType wrappingType, float textureScale, std::string textureName);

private:
	void quad(int a, int b, int c, int d, const vec3& normal);
	void generateCube();

	static const int NUM_VERTICES;
	static const vec4 CUBE_POINTS[36];
};

#endif