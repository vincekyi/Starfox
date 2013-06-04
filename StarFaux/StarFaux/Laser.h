#ifndef _LASER_H_
#define _LASER_H_

#include "Angel.h"
#include "ExternalModel.h"
#include "Vessel.h"

class Laser : public ExternalModel {
public:
	Laser(GLuint program, Vessel* vessel, Camera* camera);
	void init();
	void kill();
	void laser_update();
	bool dead();
	vec3 m_velocity;
	int m_lifespan;
	int m_timeAlive;
	Vessel* m_vessel;
	bool m_active;
};

#endif