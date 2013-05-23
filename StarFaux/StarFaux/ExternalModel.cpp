#include "ExternalModel.h"

// Constructor
ExternalModel::ExternalModel(GLuint program, vec4 color) {
	m_program = program;
	m_color = color;
}

// Load .obj file
// Sets vertices, normals, and texture coordinates if found, or NULL otherwise
//
// @param const char* filename	Filename for .obj model
// @param bool center			Centers the model at the origin if true
//
void ExternalModel::loadModel(const char* filename, bool center) {

	FILE* obj;
	obj = fopen(filename, "r");
	if (obj == NULL) {
		printf("Failed to open file %s\n", filename);
		return;
	}

	std::vector<vec4> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> textureCoords;
	std::vector<std::vector<int>> faces;
	std::vector<int> v;

	// Hopefully lines are no more than 80 characters (plus newline and null byte)
	const int maxLineSize = 82;
	char line[maxLineSize];

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

		if (sscanf(line, "v %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // Vertices
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
				v.push_back(index[w]);
			}
			faces.push_back(v);
			v.clear();
		}
	}

	if (center) {
		com /= vertices.size();
	}

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
				printf("Out of memory.\n");
				// Clean up
				vertices.clear();
				normals.clear();
				textureCoords.clear();
				v.clear();
				fclose(obj);
				return;
			}
		}

		if (hasVertices) {
			m_vertices[i] = vertices[faces[j][0] - 1] - (center ? com : 0);
			m_vertices[i+1] = vertices[faces[j][3] - 1] - (center ? com : 0);
			m_vertices[i+2] = vertices[faces[j][6] - 1] - (center ? com : 0);
		}
		if (hasTextureCoords) {
			m_textureCoords[i] = textureCoords[faces[j][1] - 1];
			m_textureCoords[i+1] = textureCoords[faces[j][4] - 1];
			m_textureCoords[i+2] = textureCoords[faces[j][7] - 1];
		}
		if (hasNormals) {
			m_normals[i] = normals[faces[j][2] - 1];
			m_normals[i+1] = normals[faces[j][5] - 1];
			m_normals[i+2] = normals[faces[j][8] - 1];
		}
	}

	m_numVertices = (int)currSize;

	vertices.clear();
	normals.clear();
	textureCoords.clear();
	v.clear();
	// Do these vectors need to be deleted?

	fclose(obj);
}
