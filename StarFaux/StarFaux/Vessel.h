#ifndef _VESSEL_H_
#define _VESSEL_H_

#include "Shape.h"
#include "MathExtension.h"
#include "ExternalModel.h"

class Vessel : public ExternalModel {
public:
	Vessel(GLuint program, Camera* camera, const char* baseDir, ShadingType shading);
	void setAccelerationX(float acc);
	void setAccelerationY(float acc);
	void setAccelerationZ(float acc);
	void updateMovement();
	vec3 getAcceleration();
	vec3 getVelocity();

	void shake();
	int m_health;
	static const float MAX_VELOCITY;
	static const float MAX_VELOCITY_Y;
private:
	static const float VELOCITY_DECAY;
	void updateVelocity(float dTime);
	void shakeShip();

	vec3 m_velocity;
	vec3 m_acceleration;

	float m_lastUpdateTime;

	Quaternion lastShake;
};

#endif
