#include "ExternalModel.h"

// Constructor
ExternalModel::ExternalModel(GLuint program, vec4 color) {
	m_program = program;
	m_color = color;
}

// Load .obj file
void ExternalModel::loadModel(const char* filename) {

	FILE* obj;
	obj = fopen(filename, "r");
	if (obj == NULL) {
		printf("Failed to open file %s\n", filename);
		return;
	}

	std::vector<vec4> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> textureCoords;
	std::vector<std::vector<float>> faces;

	const int maxLineSize = 80;
	char line[maxLineSize]; // Hopefully no lines are more than 80 characters
	int type = 0;
	float fl[9];

	for (; true;) {
		fgets(line, maxLineSize, obj);
		if (feof(obj)) {
			break;
		}
		if (strlen(line) <= 2) { // Hack because sscanf doesn't seem to work with a single whitespace followed by a newline
			continue;
		}

		if (sscanf(line, "v %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { //Vertices
			vertices.push_back(vec4(fl[0], fl[1], fl[2], 1.0));
		} else if (sscanf(line, "vn %f %f %f", &fl[0], &fl[1], &fl[2]) == 3) { // Normals
			normals.push_back(vec3(fl[0], fl[1], fl[2]));
		} else if (sscanf(line, "vt %f %f", &fl[0], &fl[1]) == 2) { // Texture coordinates
			textureCoords.push_back(vec2(fl[0], fl[1]));
		} else {
			if (sscanf(line, "f %f/%f/%f %f/%f/%f %f/%f/%f", &fl[0], &fl[1], &fl[2], &fl[3], &fl[4], &fl[5], &fl[6], &fl[7], &fl[8]) == 9) {
				type = 0; // Vertices, texture coordinates, and normals specified
			} else if (sscanf(line, "f %f//%f %f//%f %f//%f", &fl[0], &fl[2], &fl[3], &fl[5], &fl[6], &fl[8]) == 6) {
				type = 3; // Vertices and normals specified
				fl[1] = fl[4] = fl[7] = 1.0;
			} else if (sscanf(line, "f %f/%f %f/%f %f/%f", &fl[0], &fl[1], &fl[3], &fl[4], &fl[6], &fl[7]) == 6) {
				type = 2; // Vertices and texture coordinates specified
				fl[2] = fl[5] = fl[8] = 1.0;
			} else if (sscanf(line, "f %f %f %f", &fl[0], &fl[3], &fl[6]) == 3) {
				type = 1; // Vertices specified
				fl[1] = fl[2] = fl[4] = fl[5] = fl[7] = fl[8] = 1.0;
			} else {
				continue;
			}

			std::vector<float> v;
			for (int w = 0; w < 9; w++) {
				v.push_back(fl[w]);
			}
			faces.push_back(v);
			v.clear(); // Does this vector need to be deleted?
		}
	}

	size_t maxSize = vertices.size(); // Initial size
	size_t currSize = 0;

	m_vertices = (vec4*)malloc(sizeof(vec4) * maxSize);
	m_normals = (vec3*)malloc(sizeof(vec3) * maxSize);
	m_textureCoords = (vec2*)malloc(sizeof(vec2) * maxSize);

	if (type == 1) {
		textureCoords.push_back(vec2(0.0, 0.0));
		normals.push_back(vec3(1.0, 0.0, 0.0));
	} else if (type == 2) {
		normals.push_back(vec3(1.0, 0.0, 0.0));
	} else if (type == 3) {
		textureCoords.push_back(vec2(0.0, 0.0));
	}
	for (int i = 0, j = 0; j < faces.size(); i += 3, j++) {

		currSize += 3;
		if (currSize >= maxSize) {
			maxSize = 2 * maxSize; // Double the size
			m_vertices = (vec4*)realloc(m_vertices, sizeof(vec4) * maxSize);
			m_normals = (vec3*)realloc(m_normals, sizeof(vec3) * maxSize);
			m_textureCoords = (vec2*)realloc(m_textureCoords, sizeof(vec2) * maxSize);
			if (m_vertices == NULL || m_normals == NULL || m_textureCoords == NULL) {
				printf("Out of memory.\n");
				vertices.clear();
				normals.clear();
				textureCoords.clear();
				fclose(obj);
				return;
			}
		}
			
		m_vertices[i] = vertices[faces[j][0] - 1];
		m_vertices[i+1] = vertices[faces[j][3] - 1];
		m_vertices[i+2] = vertices[faces[j][6] - 1];

		m_textureCoords[i] = textureCoords[faces[j][1] - 1];
		m_textureCoords[i+1] = textureCoords[faces[j][4] - 1];
		m_textureCoords[i+2] = textureCoords[faces[j][7] - 1];
			
		m_normals[i] = normals[faces[j][2] - 1];
		m_normals[i+1] = normals[faces[j][5] - 1];
		m_normals[i+2] = normals[faces[j][8] - 1];
			
	}

	m_numVertices = (int)currSize;

	vertices.clear();
	normals.clear();
	textureCoords.clear();
	// Do these vectors need to be deleted?

	fclose(obj);
}
