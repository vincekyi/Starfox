#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "Shape.h"
#include "MathExtension.h"
#include "ExternalModel.h"

class Background : public ExternalModel {
public:
	Background(GLuint program, Camera* camera, const char* baseDir, ShadingType shading) : ExternalModel(program, baseDir, shading) {
		m_camera = camera;
		m_shakeCount = 0;
	}
};

#endif
