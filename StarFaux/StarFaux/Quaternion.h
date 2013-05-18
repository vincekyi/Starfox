/*
	Courtesy of Kasey Carrothers
*/

#ifndef _QUATERNION_H_
#define _QUATERNION_H_
 
#include "Angel.h" 

class Quaternion {
public:
	Quaternion();
	Quaternion(vec3 axis, GLfloat angle);
	Quaternion(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
 
	Quaternion operator* (Quaternion q);
	vec3 operator* (vec3 v);
 
	mat4 generateMatrix();
	Quaternion conjugate();
	void qNormalize();
 
private:
	GLfloat x, y, z, w;
	GLfloat tolerance;
};
 
#endif
