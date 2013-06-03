#ifndef _LASER_H_
#define _LASER_H_
#include "ExternalModel.h"

class Laser : public ExternalModel {
public:
	//Laser(GLuint program);
	vec3 velocity;
};

#endif