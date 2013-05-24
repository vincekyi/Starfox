#include "Shape.h"

Shape::Shape() {
	m_useTexture = 0;
	m_scale = 1.0;
	m_position = vec3(0.0, 0.0, 0.0);
	m_modified = false;
}

Shape::~Shape() {
	delete [] m_vertices;
	delete [] m_normals;
}

void Shape::initDraw() {
	glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    
    glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0])) * m_numVertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0]) * m_numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_numVertices, sizeof(m_normals[0]) * m_numVertices, m_normals);

    GLuint position = glGetAttribLocation(m_program, "vPosition");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); 

	GLuint normal = glGetAttribLocation(m_program, "vNormal");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0]) * m_numVertices));

	if (m_useTexture) {
		glGenBuffers(1, &m_textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_textureCoords[0]) * m_numVertices, m_textureCoords, GL_STATIC_DRAW );
		GLuint vTexCoords = glGetAttribLocation(m_program, "vTexCoords");
		glEnableVertexAttribArray(vTexCoords);
		glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Set up textures
		if (!m_textureImage.loadTGA(m_textureName.c_str())) {
			std::cerr << "Error loading texture image file: " << m_textureName << std::endl;
			exit(1);
		}

		glGenTextures(1, &m_textureObject);
		glBindTexture(GL_TEXTURE_2D, m_textureObject);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, m_textureImage.width, m_textureImage.height, 0,
					 (m_textureImage.byteCount == 3) ? GL_BGR : GL_BGRA,
					 GL_UNSIGNED_BYTE, m_textureImage.data);
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

void Shape::update() {
	if (m_modified) {
		m_objectToWorld = Translate(m_position) * m_qRotation.generateMatrix() * Scale(m_scale);
		m_modified = false;
	}
}
void Shape::draw(DrawType type, Camera& camera, Light& light) {
	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

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
	
	update();
	mat4 model = m_objectToWorld;
	mat4 mv = camera.worldToCamera() * model;

	glUniform4fv(uCameraPosition, 1, camera.m_position);
	glUniform4fv(uLightPosition, 1, light.m_position);
	glUniform1i(uShadingType, m_shading);
	glUniform1f(uShininess, m_shininess);
	glUniform4fv(uSpecularColor, 1, m_specularColor);
	glUniform4fv(uColor, 1, m_color);
	glUniformMatrix4fv(uProj, 1, GL_TRUE, camera.perspective());
	glUniformMatrix4fv(uModelView , 1, GL_TRUE, mv);
	glUniformMatrix4fv(uModel, 1, GL_TRUE, model);

	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	glUniform1i(uEnableTexture, 1);
	glUniform1i(uTexture, 0);

	if (m_useTexture) {

	} else {
		glUniform1i(uEnableTexture, 0);
	}

	switch (type) {
		case MESH:
			glDrawArrays(GL_LINES, 0, m_numVertices);
			break;
		case FILLED:
			glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
			break;
    }
}

void Shape::rotate(Quaternion q) {
	m_qRotation = q * m_qRotation;
	m_modified = true;
}

void Shape::resetRotation() {
	m_qRotation = Quaternion();
	m_modified = true;
}

void Shape::setupTexture(TextureSamplingType samplingType, TextureWrappingType wrappingType, std::string textureName) {
	m_useTexture = true;
	m_samplingType = samplingType;
	m_wrappingType = wrappingType;
	m_textureName = textureName;
}

void Shape::setupLighting(ShadingType shading, float shininess, vec4 specularColor) {
	m_shading = shading;
	m_shininess = shininess;
	m_specularColor = specularColor;
}

void Shape::scale(float amount) {
	m_scale = amount;
	m_modified = true;
}

void Shape::translate(float x, float y, float z) {
	m_position += vec3(x, y, z);
	m_modified = true;
}

void Shape::resetTranslation() {
	m_position = vec3(0.0f);
	m_modified = true;
}
