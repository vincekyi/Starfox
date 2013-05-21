#ifndef _VESSEL_H_
#define _VESSEL_H_

#include "Shape.h"
#include "MathExtension.h"

class Vessel {
public:
	Vessel(Camera* camera);
	void setAccelerationX(float acc);
	void setAccelerationY(float acc);
	void setAccelerationZ(float acc);
	void updateMovement();
	vec3 getAcceleration();
	vec3 getVelocity();

private:
	static const float MAX_VELOCITY;
	static const float VELOCITY_DECAY;
	void updateVelocity(float dTime);

	Shape m_shipModel;
	vec3 m_velocity;
	vec3 m_acceleration;

	float m_lastUpdateTime;

	Camera* m_camera;
};

#endif