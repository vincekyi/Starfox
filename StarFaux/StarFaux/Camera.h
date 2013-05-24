#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Quaternion.h"
#include "Angel.h"

class Camera {
public:
	Camera();

	// Initializes the orientation of the camera and creates projections
	void init(double fovy, double aspect, double zNear, double zFar);

	// Set the orientation of the camera
	void lookAt(vec3 look, vec3 up);

	// Returns the perspective projection stored in the camera
	mat4 perspective();

	void rotateYaw(float theta);
	void rotatePitch(float theta);
	void rotateRoll(float theta);
	void rotateQuat(Quaternion q);
	void translate(vec3 v);
	void translate(float dx, float dy, float dz);

	mat4 worldToCamera();

	void update();

	// Location of the camera in the world
	vec3 m_position;
	Quaternion m_qRotation;

	// The camera axes
	vec3 m_lookAt;
	vec3 m_xAxis;
	vec3 m_yAxis;
	vec3 m_zAxis;

	// Projection attributes
	double m_fovy;
	double m_aspect;
	double m_zNear;
	double m_zFar;
	
private:
	static const vec3 WORLD_XAXIS;
	static const vec3 WORLD_YAXIS;
	static const vec3 WORLD_ZAXIS;

	mat4 m_worldToCamera;
	mat4 m_perspective;

	

	bool m_modified;
};

#endif
