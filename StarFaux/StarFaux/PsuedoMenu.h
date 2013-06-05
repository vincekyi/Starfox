#ifndef _PSUEDOMENU_H_
#define _PSUEDOMENU_H_

#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Types.h"
#include "Shape.h"
#include <string>

class PsuedoMenu : public Shape {
public:
	PsuedoMenu(GLuint program, ShadingType shading, float height, float width);
private:
	void generateMenu(float height, float width);

	const static int NUMPOINTS = 6;
	const static vec4 squarePoints[NUMPOINTS];
};

#endif