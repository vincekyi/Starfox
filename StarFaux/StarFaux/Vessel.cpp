#include "Vessel.h"

const float Vessel::MAX_VELOCITY = 2.0f;
const float Vessel::VELOCITY_DECAY = 0.08f;

Vessel::Vessel(Camera* camera) {
	m_lastUpdateTime = 0;
	m_acceleration = vec3(0.0);
	m_velocity = vec3(0.0);
	m_camera = camera;
}
void Vessel::setAccelerationX(float acc) {
	m_acceleration.x = acc;
}

void Vessel::setAccelerationY(float acc) {
	m_acceleration.y = acc;
}

void Vessel::setAccelerationZ(float acc) {
	m_acceleration.z = acc;
}

void Vessel::updateMovement() {
	if (m_lastUpdateTime == 0) {
		m_lastUpdateTime = glutGet(GLUT_ELAPSED_TIME);
		return;
	}

	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	float dTime = (currentTime - m_lastUpdateTime) / 10;
	
	// x_new - x_old = vt + 0.5at^2
	vec3 dPosition = dTime * m_velocity + 0.5 * m_acceleration * dTime * dTime;
	m_camera->translate(dPosition);

	updateVelocity(dTime);
	m_lastUpdateTime = currentTime;
}

void Vessel::updateVelocity(float dTime) {
	// v_new = v_old + at
	m_velocity += dTime * m_acceleration;
	if (m_velocity.x > MAX_VELOCITY) 
		m_velocity.x = MAX_VELOCITY;
	else if (m_velocity.x < -MAX_VELOCITY) 
		m_velocity.x = -MAX_VELOCITY;
	if (m_velocity.y > MAX_VELOCITY) m_velocity.y = MAX_VELOCITY;
	else if (m_velocity.y < -MAX_VELOCITY) m_velocity.y = -MAX_VELOCITY;
	if (m_velocity.z > MAX_VELOCITY)
		m_velocity.z = MAX_VELOCITY;
	else if (m_velocity.z < -MAX_VELOCITY)
		m_velocity.z = -MAX_VELOCITY;

	// decay velocity to 0 if no acceleration
	if (Math::floatEquality(m_acceleration.x, 0.0f)) {
		if (!Math::floatEquality(m_velocity.x, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.x);
			m_velocity.x += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.x < 0 && velocitySign > 0) || (m_velocity.x > 0 && velocitySign < 0))
				m_velocity.x = 0.0f;
		}
	}
	if (Math::floatEquality(m_acceleration.y, 0.0f)) {
		if (!Math::floatEquality(m_velocity.y, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.y);
			m_velocity.y += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.y < 0 && velocitySign > 0) || (m_velocity.y > 0 && velocitySign < 0))
				m_velocity.y = 0.0f;
		}
	}
	if (Math::floatEquality(m_acceleration.z, 0.0f)) {
		if (!Math::floatEquality(m_velocity.z, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.z);
			m_velocity.z += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.z < 0 && velocitySign > 0) || (m_velocity.z > 0 && velocitySign < 0))
				m_velocity.z = 0.0f;
		}
	}
}

vec3 Vessel::getAcceleration() {
	return m_acceleration;
}

vec3 Vessel::getVelocity() {
	return m_velocity;
}
