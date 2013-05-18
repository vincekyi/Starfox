#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Angel.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Light.h"
#include "Types.h"
#include "Tga.h"
#include <string>

class Shape {
public:
	Shape();
	~Shape();

	void draw(DrawType type, Camera& camera, Light& light);
	void rotate(Quaternion q);
	void scale(float amount);
	void rotateTexture(float theta);
	void translateTexture(vec2 v);
	vec3 m_position;
	mat4 m_oToW;
	mat3 m_textureTrans;

protected:
	void initDraw();

	// Transformations
	Quaternion m_qRotation;
	float m_scale;

	ShadingType m_shading;
	float m_shininess;
	vec4 m_specularColor;
	vec4 m_color;

	int m_numVertices;
	int m_vertexIndex;
	vec4* m_vertices;
	vec3* m_normals;
	vec2* m_textureCoords;
	bool m_useTexture;
	std::string m_textureName;
	TgaImage m_textureImage;
	TextureSamplingType m_samplingType;
	TextureWrappingType m_wrappingType;
	float m_textureScale;

	GLuint m_program;
	GLuint m_vertexArrayObject;                      
    GLuint m_vertexBuffer;
	GLuint m_textureObject;
	GLuint m_textureBuffer;
};

#endif