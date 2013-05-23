#ifndef _EXTERNALMODEL_H_
#define _EXTERNALMODEL_H_

#include "Angel.h"
#include "Types.h"
#include "Shape.h"
#include <stdio.h>
#include <string.h>
#include <vector>

class ExternalModel : public Shape {
public:
	ExternalModel(GLuint program, vec4 color);
	void loadModel(const char* filename, bool center);
};

#endif
