#include "Camera.h"

const vec3 Camera::WORLD_XAXIS(1.0, 0.0, 0.0);
const vec3 Camera::WORLD_YAXIS(0.0, 1.0, 0.0);
const vec3 Camera::WORLD_ZAXIS(0.0, 0.0, 1.0);

Camera::Camera() {
	m_position = vec3(0.0, 0.0, 0.0);
	lookAt(vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
	m_qRotation = Quaternion();
	m_modified = 1;
}

void Camera::init(double fovy, double aspect, double zNear, double zFar) {
	m_fovy = fovy;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;
	m_worldToCamera = mat4();
	m_perspective = Perspective(fovy, aspect, zNear, zFar);
	m_modified = 1;
}

void Camera::lookAt(vec3 look, vec3 up) {
	m_zAxis = normalize(m_position - look);
	m_xAxis = normalize(cross(up, m_zAxis));
	m_yAxis = normalize(cross(m_zAxis, m_xAxis));
	m_lookAt = -m_zAxis;
	m_modified = 1;
}

void Camera::rotateYaw(float theta) {
	Quaternion yaw = Quaternion(WORLD_YAXIS, -theta);
	m_qRotation = yaw * m_qRotation;
	m_modified = 1;
}

void Camera::rotatePitch(float theta) {
	Quaternion pitch = Quaternion(m_xAxis, -theta);
	m_qRotation = m_qRotation * pitch;
	m_modified = 1;
}

void Camera::translate(vec3 v) {
	vec3 dx = v.x * m_xAxis;
	vec3 dy = v.y * m_yAxis;
	vec3 dz = v.z * m_zAxis;
	m_position += dx + dy + dz;
	m_modified = 1;
}

void Camera::translate(float x, float y, float z) {
	vec3 dx = x * m_xAxis;
	vec3 dy = y * m_yAxis;
	vec3 dz = z * m_zAxis;
	m_position += dx + dy + dz;
	m_modified = 1;
}

mat4 Camera::worldToCamera() {
	return m_worldToCamera;
}


mat4 Camera::perspective() {
	return m_perspective;
}

void Camera::update() {
	if (m_modified) {
		mat4 rot = m_qRotation.generateMatrix();

		m_xAxis = vec3(rot[0][0], rot[0][1], rot[0][2]);
		m_yAxis = vec3(rot[1][0], rot[1][1], rot[1][2]);
		m_zAxis = vec3(rot[2][0], rot[2][1], rot[2][2]);
		m_lookAt = -m_zAxis;

		m_worldToCamera = rot * Translate(-m_position);
		m_modified = 0;
	}
}
