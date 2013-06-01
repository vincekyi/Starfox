#ifndef _EXTERNALMODEL_H_
#define _EXTERNALMODEL_H_

#include "Angel.h"
#include "Types.h"
#include "Shape.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>

class ExternalModel : public Shape {
public:
	ExternalModel(GLuint program, const char* baseDir);
	void loadModel(const char* filename, bool center);

	void initDraw();
	void draw(DrawType type, Camera& camera, Light& light);
	void setupTexture(TextureSamplingType samplingType, TextureWrappingType wrappingType);
	using Shape::setupTexture;
	void setupLighting(ShadingType shading);
	using Shape::setupLighting;
protected:
	Camera* m_camera;

	bool m_hasMaterials;
	struct materialProp_t {
		vec3 Ka;
		vec3 Kd;
		vec3 Ks;
		float Ns;

		char* map_Ka;
		char* map_Kd;
		char* map_Ks;
		char* map_Ns;
	};

	struct cmp_str {
		bool operator()(const char* a, const char* b) {
			return strcmp(a, b) < 0;
		}
	};

	static const int maxLineSize = 82;
	char* m_baseDir;
	std::map<int, char*> m_materialRefs;
	std::map<char*, materialProp_t*, cmp_str> m_textureMaps;

	// Need multiple vertex and texture buffers
	GLuint* m_vertexArrayObjectArray;
	GLuint* m_vertexBufferArray;
	GLuint* m_textureBufferArray;
	GLuint* m_textureObjectArray;

	void loadTextureMaps(const char* filename);
	char* getTextureMap(materialProp_t* mpt);
	bool mtl_parse_error(materialProp_t* m, FILE* f, char* fn);
};

#endif
