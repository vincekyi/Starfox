#include "ExternalModel.h"

// Constructor
//
// @param const char* baseDir	
//		Base directory for model files (.obj, .mtl, .tga)
//
ExternalModel::ExternalModel(GLuint program, vec4 color, const char* baseDir) {
	m_program = program;
	m_color = color;
	m_hasMaterials = false;

	if (baseDir[strlen(baseDir)] != '/') {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + 2 * sizeof(char));
		strcpy(m_baseDir, baseDir);
		strcat(m_baseDir, "/");
	} else {
		m_baseDir = (char*)malloc(sizeof(char) * strlen(baseDir) + sizeof(char));
		strcpy(m_baseDir, baseDir);
	}
}

// Overloaded setupTexture; this class should not use m_textureName if there is a .mtl file
void ExternalModel::setupTexture(TextureSamplingType samplingType, TextureWrappingType wrappingType) {
	Shape::setupTexture(samplingType, wrappingType, "");
}

// Overloaded initDraw
// Set up each texture
void ExternalModel::initDraw() {
	if (!m_useTexture || !m_hasMaterials) {
		Shape::initDraw();
		return;
	}

	m_vertexArrayObjectArray = (GLuint*)malloc(sizeof(GLuint) * m_materialRefs.size());
	m_vertexBufferArray = (GLuint*)malloc(sizeof(GLuint) * m_materialRefs.size());
	m_textureBufferArray = (GLuint*)malloc(sizeof(GLuint) * m_materialRefs.size());
	m_textureObjectArray = (GLuint*)malloc(sizeof(GLuint) * m_materialRefs.size());
	glGenVertexArrays(m_materialRefs.size(), m_vertexArrayObjectArray);
	glGenBuffers(m_materialRefs.size(), m_vertexBufferArray);
	glGenBuffers(m_materialRefs.size(), m_textureBufferArray);
	glGenTextures(m_materialRefs.size(), m_textureObjectArray);

	TgaImage* textureImages = (TgaImage*)malloc(sizeof(TgaImage) * m_materialRefs.size());

	int i = 0;
	for (std::map<int, char*>::iterator iter = m_materialRefs.begin(); iter != m_materialRefs.end(); iter++, i++) {

		iter++;
		int range = 0;
		int offset = 0;
		if (iter == m_materialRefs.end()) {
			iter--;
			range = m_numVertices/3 - iter->first;
			offset = iter->first;
			iter++;
		} else {
			range = iter->first;
			iter--;
			offset = iter->first;
			iter++;
		}
		iter--;
		range *= 3; offset *= 3;

		glBindVertexArray(m_vertexArrayObjectArray[i]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferArray[i]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3)) * range, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * range, &m_vertices[offset]); 
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * range, sizeof(vec3) * range, &m_normals[offset]);

		GLuint position = glGetAttribLocation(m_program, "vPosition");
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); 
		GLuint normal = glGetAttribLocation(m_program, "vNormal");
		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * range));
		
		glBindBuffer(GL_ARRAY_BUFFER, m_textureBufferArray[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * range, &m_textureCoords[offset], GL_STATIC_DRAW );

		GLuint vTexCoords = glGetAttribLocation(m_program, "vTexCoords");
		glEnableVertexAttribArray(vTexCoords);
		glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		
		textureImages[i] = TgaImage(); // needed?
		char* textureMap = getTextureMap(m_textureMaps[iter->second]);
		if (textureMap == NULL)
			return;
		char* img_fn = (char*)malloc(sizeof(char)*strlen(m_baseDir) + sizeof(char)*strlen(textureMap) + sizeof(char));
		strcpy(img_fn, m_baseDir);
		strcat(img_fn, textureMap);
		if (!textureImages[i].loadTGA(img_fn)) {
			fprintf(stderr, "Error loading texture image file: %s\n", iter->second);
			return;
		}
		
		glBindTexture(GL_TEXTURE_2D, m_textureObjectArray[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, textureImages[i].width, textureImages[i].height, 0,
					 (textureImages[i].byteCount == 3) ? GL_BGR : GL_BGRA,
					 GL_UNSIGNED_BYTE, textureImages[i].data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		if (m_wrappingType == CLAMP) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		} else if (m_wrappingType == REPEAT) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		if (m_samplingType == NEAREST_NEIGHBOR) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		} else if (m_samplingType == BILINEAR) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		} else if (m_samplingType == TRILINEAR) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
	}
}

// Overloaded draw
// Process each texture
void ExternalModel::draw(DrawType type, Camera& camera, Light& light) {
	if (!m_useTexture || !m_hasMaterials || m_textureCoords == NULL) {
		Shape::draw(type, camera, light);
		return;
	}

	GLuint uCameraPosition = glGetUniformLocation(m_program, "uCameraPosition");
	GLuint uLightPosition = glGetUniformLocation(m_program, "uLightPosition");
	GLuint uShadingType = glGetUniformLocation(m_program, "uShadingType");
	GLuint uShininess = glGetUniformLocation(m_program, "uShininess");
	GLuint uSpecularColor = glGetUniformLocation(m_program, "uSpecularColor");
	GLuint uColor = glGetUniformLocation(m_program, "uColor");
	GLuint uProj = glGetUniformLocation(m_program, "uProj");
	GLuint uModelView = glGetUniformLocation(m_program, "uModelView");
	GLuint uModel = glGetUniformLocation(m_program, "uModel");
	GLuint uEnableTexture = glGetUniformLocation(m_program, "uEnableTexture");
	GLuint uTexture = glGetUniformLocation(m_program, "uTexture");
	glUniform4fv(uCameraPosition, 1, camera.m_position);
	glUniform4fv(uLightPosition, 1, light.m_position);
	glUniformMatrix4fv(uProj, 1, GL_TRUE, camera.perspective());
	glUniform1i(uShadingType, m_shading);
	glUniform4fv(uColor, 1, m_color);
	glUniform1i(uEnableTexture, 1);
	glUniform1i(uTexture, 0);

	int i = 0;
	for (std::map<int, char*>::iterator iter = m_materialRefs.begin(); i < m_materialRefs.size(); iter++, i++) {
		glBindVertexArray(m_vertexArrayObjectArray[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferArray[i]);

		if (m_modified) {
			m_objectToWorld = Translate(m_position) * m_qRotation.generateMatrix() * Scale(m_scale);
			m_modified = false;
		}
		mat4 model = m_objectToWorld;
		mat4 mv = camera.worldToCamera() * model;
		
		glUniformMatrix4fv(uModelView , 1, GL_TRUE, mv);
		glUniformMatrix4fv(uModel, 1, GL_TRUE, model);

		glUniform1f(uShininess, m_textureMaps[iter->second]->Ns);
		glUniform4fv(uSpecularColor, 1, m_textureMaps[iter->second]->Ks);

		glBindBuffer(GL_ARRAY_BUFFER, m_textureBufferArray[i]);
		glBindTexture(GL_TEXTURE_2D, m_textureObjectArray[i]);

		iter++;
		int range = 0;
		if (iter == m_materialRefs.end()) {
			iter--;
			range = m_numVertices/3 - iter->first;
			iter++;
		} else {
			range = iter->first;
		}
		range *= 3;
		glDrawArrays(GL_TRIANGLES, 0, range);
		iter--;
	}
}

// Load .obj file
// Sets vertices, normals, and texture coordinates if found, or NULL otherwise
//
// @param const char* filename	Filename for .obj model, relative to m_baseDir
// @param bool center			Centers the model at the origin if true
//
void ExternalModel::loadModel(const char* filename, bool center) {
	char* obj_fn = (char*)malloc(sizeof(char) * strlen(m_baseDir) + sizeof(char) * strlen(filename) + sizeof(char));
	strcpy(obj_fn, m_baseDir);
	strcat(obj_fn, filename);

	FILE* obj;
	obj = fopen(obj_fn, "r");
	if (obj == NULL) {
		fprintf(stderr, "Failed to open file: %s\n", obj_fn);
		free(obj_fn);
		return;
	}
	free(obj_fn);

	std::vector<vec4> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> textureCoords;
	std::vector<std::vector<int>> faces;
	std::vector<int> v;

	// Hopefully lines are no more than 80 characters (plus newline and null byte)
	char line[maxLineSize];
	char* mtllib_fn = (char*)malloc(sizeof(char) * maxLineSize);
	char mat[maxLineSize];
	bool hasVertices, hasNormals, hasTextureCoords;
	hasVertices = hasNormals = hasTextureCoords = false;
	float fl[3];
	int index[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	vec4 com = vec4(0.0, 0.0, 0.0, 0.0); // Model's center of mass

	for (; true;) {
		fgets(line, maxLineSize, obj);
		if (feof(obj)) {
			break;
		}

		if (sscanf(line, "mtllib %s", mtllib_fn)) { // material library
			m_hasMaterials = true;
		} else if (sscanf(line, "usemtl %s", mat)) { // material reference
			char* tm = (char*)malloc(sizeof(char) * strlen(mat) + sizeof(char));
			strcpy(tm, mat);
			m_materialRefs.insert(std::pair<int, char*>(faces.size(), tm));
		} else if (sscanf(line, "v %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // Vertices
			vertices.push_back(vec4(fl[0], fl[1], fl[2], 1.0));
			com += vec4(fl[0], fl[1], fl[2], 0.0);
		} else if (sscanf(line, "vn %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // Normals
			normals.push_back(vec3(fl[0], fl[1], fl[2]));
		} else if (sscanf(line, "vt %f %f", &fl[0], &fl[1]) == 2) { // Texture coordinates
			textureCoords.push_back(vec2(fl[0], fl[1]));
		} else {
			if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
					&index[0], &index[1], &index[2], &index[3], &index[4], &index[5], &index[6], &index[7], &index[8]) == 9) {
				hasVertices = hasNormals = hasTextureCoords = true;
			} else if (sscanf(line, "f %d//%d %d//%d %d//%d", 
					&index[0], &index[2], &index[3], &index[5], &index[6], &index[8]) == 6) {
				hasVertices = hasNormals = true;
			} else if (sscanf(line, "f %d/%d %d/%d %d/%d", &index[0], &index[1], &index[3], &index[4], &index[6], &index[7]) == 6) {
				hasVertices = hasTextureCoords = true;
			} else if (sscanf(line, "f %d %d %d", &index[0], &index[3], &index[6]) == 3) {
				hasVertices = true;
			} else {
				continue;
			}

			for (int w = 0; w < 9; w++) {
				int ind = index[w] < 0 ? vertices.size() + index[w] : index[w] - 1;
				v.push_back(ind);
			}
			faces.push_back(v);
			v.clear();
		}
	}
	
	com /= vertices.size(); // Calculate center of mass

	size_t maxSize = vertices.size(); // Initial size (arbitrary)
	size_t currSize = 0;

	m_vertices = hasVertices ? (vec4*)malloc(sizeof(vec4) * maxSize) : NULL;
	m_normals = hasNormals ? (vec3*)malloc(sizeof(vec3) * maxSize) : NULL;
	m_textureCoords = hasTextureCoords ? (vec2*)malloc(sizeof(vec2) * maxSize) : NULL;

	for (int i = 0, j = 0; j < faces.size(); i += 3, j++) {
		currSize += 3;
		if (currSize >= maxSize) {
			maxSize = 2 * maxSize; // Double the size
			m_vertices = (vec4*)realloc(m_vertices, sizeof(vec4) * maxSize);
			m_normals = (vec3*)realloc(m_normals, sizeof(vec3) * maxSize);
			m_textureCoords = (vec2*)realloc(m_textureCoords, sizeof(vec2) * maxSize);
			if (m_vertices == NULL || m_normals == NULL || m_textureCoords == NULL) {
				fprintf(stderr, "Unable to allocate memory.\n");
				vertices.clear(); normals.clear(); textureCoords.clear(); v.clear(); fclose(obj); 
				if (m_hasMaterials) {
					free(mtllib_fn);
				}
				return;
			}
		}

		if (hasVertices) {
			m_vertices[i] = vertices[faces[j][0]] - (center ? com : 0);
			m_vertices[i+1] = vertices[faces[j][3]] - (center ? com : 0);
			m_vertices[i+2] = vertices[faces[j][6]] - (center ? com : 0);
		}
		if (hasTextureCoords) {
			m_textureCoords[i] = textureCoords[faces[j][1]];
			m_textureCoords[i+1] = textureCoords[faces[j][4]];
			m_textureCoords[i+2] = textureCoords[faces[j][7]];
		}
		if (hasNormals) {
			m_normals[i] = normals[faces[j][2]];
			m_normals[i+1] = normals[faces[j][5]];
			m_normals[i+2] = normals[faces[j][8]];
		}
	}

	m_numVertices = (int)currSize;

	vertices.clear();
	normals.clear();
	textureCoords.clear();
	v.clear();

	fclose(obj);

	if (m_hasMaterials) {
		loadTextureMaps(mtllib_fn);
	}
	free(mtllib_fn);
}

// Load texture maps from .mtl file
void ExternalModel::loadTextureMaps(const char* filename) {
	char* mtllib_fn = (char*)malloc(sizeof(char) * strlen(m_baseDir) + sizeof(char) * strlen(filename) + sizeof(char));
	strcpy(mtllib_fn, m_baseDir);
	strcat(mtllib_fn, filename);

	FILE* mtllib;
	mtllib = fopen(mtllib_fn, "r");
	if (mtllib == NULL) {
		fprintf(stderr, "Failed to open file: %s\n", mtllib_fn);
		free(mtllib_fn);
		return;
	}

	char line[maxLineSize];
	char txt[maxLineSize];
	float fl[3];
	materialProp_t* mp = NULL;

	for (; true;) {
		fgets(line, maxLineSize, mtllib);
		if (feof(mtllib)) {
			break;
		}

		if (sscanf(line, "newmtl %s", txt)) { // material reference
			char* mat = (char*)malloc(sizeof(char) * strlen(txt) + sizeof(char));
			strcpy(mat, txt);
			mp = (materialProp_t*)malloc(sizeof(materialProp_t));
			mp->map_Ka = NULL; mp->map_Kd = NULL; mp->map_Ks = NULL; mp->map_Ns = NULL; // Make an initialization func?
			m_textureMaps.insert(std::pair<char*, materialProp_t*>(mat, mp));
		} else if (sscanf(line, "map_Ka %s", txt)) { // ambient texture map
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			char* img = (char*)malloc(sizeof(char) * strlen(txt) + sizeof(char));
			strcpy(img, txt);
			mp->map_Ka = img;
		} else if (sscanf(line, "map_Kd %s", txt)) { // diffuse texture map
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			char* img = (char*)malloc(sizeof(char) * strlen(txt) + sizeof(char));
			strcpy(img, txt);
			mp->map_Kd = img;
		} else if (sscanf(line, "map_Ks %s", txt)) { // specular color texture map
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			char* img = (char*)malloc(sizeof(char) * strlen(txt) + sizeof(char));
			strcpy(img, txt);
			mp->map_Ks = img;
		} else if (sscanf(line, "map_Ns %s", txt)) { // specular highlight component
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			char* img = (char*)malloc(sizeof(char) * strlen(txt) + sizeof(char));
			strcpy(img, txt);
			mp->map_Ns = img;
		} else if (sscanf(line, "Ka %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // ambient coefficient
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			mp->Ka = vec3(fl[0], fl[1], fl[2]);
		} else if (sscanf(line, "Ks %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // specular coefficient
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			mp->Ks = vec3(fl[0], fl[1], fl[2]);
		} else if (sscanf(line, "Kd %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // diffuse coefficient
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			mp->Kd = vec3(fl[0], fl[1], fl[2]);
		} else if (sscanf(line, "Ns %f", &fl[0])) { // Shininess
			if (mtl_parse_error(mp, mtllib, mtllib_fn)) 
				return;
			mp->Ns = fl[0];
		}
	}

	fclose(mtllib);
	free(mtllib_fn);
}

// TODO: Somehow combine texture maps instead of arbitrarily choosing one?
char* ExternalModel::getTextureMap(materialProp_t* mpt) {
	char* textureMap;
	if (mpt->map_Ka != NULL) {
		textureMap = (char*)malloc(sizeof(char) * strlen(mpt->map_Ka) + sizeof(char));
		return strcpy(textureMap, mpt->map_Ka);
	} else if (mpt->map_Kd != NULL) {
		textureMap = (char*)malloc(sizeof(char) * strlen(mpt->map_Kd) + sizeof(char));
		return strcpy(textureMap, mpt->map_Kd);
	} else if (mpt->map_Ks != NULL) {
		textureMap = (char*)malloc(sizeof(char) * strlen(mpt->map_Ks) + sizeof(char));
		return strcpy(textureMap, mpt->map_Ks);
	} else if (mpt->map_Ns != NULL) {
		textureMap = (char*)malloc(sizeof(char) * strlen(mpt->map_Ns) + sizeof(char));
		return strcpy(textureMap, mpt->map_Ns);
	}

	fprintf(stderr, "Error: No texture maps found.\n");
	return NULL;
}

// Convenience function
bool ExternalModel::mtl_parse_error(materialProp_t* m, FILE* f, char* fn) {
	if (m == NULL) {
		fprintf(stderr, "Parse error in file: %s\n", fn);
		fclose(f);
		free(fn);
		return false;
	}
	return false;
}
