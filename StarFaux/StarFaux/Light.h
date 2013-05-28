#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Types.h"

class Light {
public:
	vec4 m_position;
	vec4 m_lightAmbient;
	vec4 m_lightDiffuse;
	vec4 m_lightSpecular;
private:
};

#endif
