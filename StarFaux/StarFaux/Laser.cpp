#include "Laser.h"

Laser::Laser(GLuint program, Vessel* vessel, Camera* camera) : ExternalModel(program, "./models/laser", NONE) {
	loadModel("laser2.obj", true);
	setupLighting(20.0, vec4(0.1, 0.9, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0), vec4(0.1, 1.0, 0.1, 1.0));
	setupTexture(NO_TEXTURE, TRILINEAR, REPEAT);
	initDraw();
	m_camera = camera;
	m_vessel = vessel;
	setPosition(m_camera->m_position - m_camera->m_zAxis * 12.0f - m_camera->m_yAxis * 2.0f);
	translate(0.0f, 1.4 * m_vessel->getVelocity().y / m_vessel->MAX_VELOCITY_Y, 0.0f);
	vec3 a = Quaternion(vec3(0.0f, -1.0f, 0.0f), 70.0f * (m_vessel->getVelocity().x / m_vessel->MAX_VELOCITY)) * m_camera->m_zAxis;
	vec3 b = Quaternion(m_camera->m_xAxis, 60.0f * (m_vessel->getVelocity().y / m_vessel->MAX_VELOCITY_Y)) * m_camera->m_zAxis;
	Quaternion rot = m_camera->m_qRotation;
	rot.w = -m_camera->m_qRotation.w;
	resetRotation();
	rotate(rot*m_vessel->m_qRotation);
	m_velocity = -10.0 * (a + b);
	m_lifespan = 50;
	m_timeAlive = 0;
}

void Laser::laser_update() {
	translate(m_velocity.x, m_velocity.y, m_velocity.z);
	++m_timeAlive;
}

void Laser::kill() {
	m_timeAlive = 100;
}

bool Laser::dead() {
	return m_timeAlive > m_lifespan;
}