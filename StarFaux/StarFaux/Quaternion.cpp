/*
	Courtesy of Kasey Carrothers
*/

#include "Quaternion.h"

Quaternion::Quaternion() {
	//Generates the multiplicative identity quaternion
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
	tolerance = 0.000001;
}

Quaternion::Quaternion(vec3 axis, GLfloat angle) {
	//Generates a quaternion based on the rotation around the given axis.
	vec3 normAxis = normalize(axis);
	GLfloat rads = angle * DegreesToRadians;
	GLfloat sineHalfAngle = sin(rads/2.0);
 
	x = normAxis.x * sineHalfAngle;
	y = normAxis.y * sineHalfAngle;
	z = normAxis.z * sineHalfAngle;
	w = cos(rads/2.0);
 
	tolerance = 0.000001;
}

Quaternion::Quaternion(GLfloat xVal, GLfloat yVal, GLfloat zVal, GLfloat wVal) {
	//Specifies an exact quaternion.
	x = xVal;
	y = yVal;
	z = zVal;
	w = wVal;
 
	tolerance = 0.000001;
}

Quaternion Quaternion::operator* (Quaternion q) {
	//Multiplies two quaternions.

	//Order matters!
	qNormalize();
 
	GLfloat newX, newY, newZ, newW;
 
	newX = (w*q.x + x*q.w + y*q.z - z*q.y);
	newY = (w*q.y + y*q.w + z*q.x - x*q.z);
	newZ = (w*q.z + z*q.w + x*q.y - y*q.x);
	newW = (w*q.w - x*q.x - y*q.y - z*q.z);
 
	return Quaternion(newX, newY, newZ, newW);
}

vec3 Quaternion::operator* (vec3 v) {
	//Rotates vector v by the quaternion
	vec3 vec(v);
	qNormalize();
	vec = normalize(vec);
 
	Quaternion vQuat(vec.x, vec.y, vec.z, 0.0);
 
	Quaternion result = vQuat * conjugate();
	result = *this * result;
 
	return vec3(result.x, result.y, result.z);
}

Quaternion Quaternion::conjugate() {
	return Quaternion(-x, -y, -z, w);
}

mat4 Quaternion::generateMatrix() {
	//Generates a rotation matrix
	qNormalize();
	mat4 matrix;
	GLfloat x2 = x * x;
	GLfloat y2 = y * y;
	GLfloat z2 = z * z;
	GLfloat xy = x * y;
	GLfloat xz = x * z;
	GLfloat yz = y * z;
	GLfloat wx = w * x;
	GLfloat wy = w * y;
	GLfloat wz = w * z;
 
	matrix[0][0] = 1.0 - 2.0 * (y2 + z2);
	matrix[0][1] = 2.0 * (xy - wz);
	matrix[0][2] = 2.0 * (xz + wy);
	matrix[1][0] = 2.0 * (xy + wz);
	matrix[1][1] = 1.0 - 2.0 * (x2 + z2);
	matrix[1][2] = 2.0 * (yz - wx);
	matrix[2][0] = 2.0 * (xz - wy);
	matrix[2][1] = 2.0 * (yz + wx);
	matrix[2][2] = 1.0 - 2.0* (x2 + y2);
 
	return matrix;
}

void Quaternion::qNormalize() {
	//Normalizes the quaternion if it exceeds the tolerance
	GLfloat mag2 = x*x + y*y + z*z + w*w;
 
	if (abs(mag2 - 1.0) > tolerance) {
		GLfloat mag = sqrt(mag2);
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
}
