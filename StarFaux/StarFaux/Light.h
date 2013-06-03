#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Types.h"

class Light {
public:
	vec4 m_position;
	vec4 m_lightAmbient;
	vec4 m_lightDiffuse;
	vec4 m_lightSpecular;
	float m_attenuation;
private:
};

struct lightEffects {
	vec4* lightPositions;
	vec4* ambientProducts;
	vec4* diffuseProducts;
	vec4* specularProducts;
	float* attenuations;
	int numLights;

};

#endif
