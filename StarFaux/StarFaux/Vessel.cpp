#include "Vessel.h"

const float Vessel::MAX_VELOCITY = 2.5f;
const float Vessel::VELOCITY_DECAY = MAX_VELOCITY / 100.0f;

Vessel::Vessel(GLuint program, vec4 color, Camera* camera) : ExternalModel(program, color) {
	m_lastUpdateTime = 0;
	m_acceleration = vec3(0.0);
	m_velocity = vec3(0.0);
	m_camera = camera;
	m_shakeCount = 0;
}

void Vessel::setAccelerationX(float acc) {
	m_acceleration.x = acc;
}

void Vessel::setAccelerationY(float acc) {
	m_acceleration.y = acc;
}

void Vessel::setAccelerationZ(float acc) {
	m_acceleration.z = acc;
}

void Vessel::shakeShip() {
	if (m_shakeCount > 55) {
		lastShake = Quaternion(vec3(0.0, 0.0, 1.0), 0.75f) * lastShake;
		rotate(lastShake);
	}
	else if (m_shakeCount % 20 < 10) {
		//vec3 tmp = vec3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
		lastShake = Quaternion(vec3(0.0, 0.0, 1.0), 4.5f) * lastShake;
		rotate(lastShake);
		//translate((rand() % 10 / 20.0) - 0.25, (rand() % 10 / 20.0) - 0.25, (rand() % 10 / 20.0) - 0.25);
		//lastShake += vec3((rand() % 10 / 100.0f) - 0.05, (rand() % 10 / 100.0f) - 0.05, (rand() % 10 / 100.0f) - 0.05);
		//translate(lastShake.x, lastShake.y, lastShake.z);
		//std::cout << "WTF" << std::endl;
	} else {
		lastShake = Quaternion(vec3(0.0, 0.0, 1.0), -4.5f) * lastShake;
		rotate(lastShake);
	}
	m_shakeCount--;
}
void Vessel::updateMovement() {
	if (m_lastUpdateTime == 0) {
		m_lastUpdateTime = glutGet(GLUT_ELAPSED_TIME);
		return;
	}

	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	float dTime = (currentTime - m_lastUpdateTime) / 10;
	
	// x_new - x_old = vt + 0.5at^2
	vec3 dPosition = dTime * m_velocity + 0.5 * m_acceleration * dTime * dTime;
	m_camera->translate(dPosition);
	updateVelocity(dTime);
	m_box->setCenter(m_camera->m_position - vec3(0.0f, 1.0f, 10.0f));
	if (m_shakeCount > 0) {
		shakeShip();
	} else {
		lastShake = Quaternion();
	}
	m_lastUpdateTime = currentTime;
}

void Vessel::updateVelocity(float dTime) {
	// v_new = v_old + at
	vec3 m_oldVelocity = m_velocity;
	m_velocity += dTime * m_acceleration;
	if (m_velocity.x > MAX_VELOCITY) m_velocity.x = MAX_VELOCITY;
	else if (m_velocity.x < -MAX_VELOCITY) m_velocity.x = -MAX_VELOCITY;
	if (m_velocity.y > MAX_VELOCITY) m_velocity.y = MAX_VELOCITY;
	else if (m_velocity.y < -MAX_VELOCITY) m_velocity.y = -MAX_VELOCITY;
	if (m_velocity.z > MAX_VELOCITY) m_velocity.z = MAX_VELOCITY;
	else if (m_velocity.z < -MAX_VELOCITY) m_velocity.z = -MAX_VELOCITY;


	// decay velocity to 0 if no acceleration
	if (Math::floatEquality(m_acceleration.x, 0.0f)) {
		if (!Math::floatEquality(m_velocity.x, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.x);
			m_velocity.x += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.x < 0 && velocitySign > 0) || (m_velocity.x > 0 && velocitySign < 0))
				m_velocity.x = 0.0f;
		}
	}
	if (Math::floatEquality(m_acceleration.y, 0.0f)) {
		if (!Math::floatEquality(m_velocity.y, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.y);
			m_velocity.y += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.y < 0 && velocitySign > 0) || (m_velocity.y > 0 && velocitySign < 0))
				m_velocity.y = 0.0f;
		}
	}
	if (Math::floatEquality(m_acceleration.z, 0.0f)) {
		if (!Math::floatEquality(m_velocity.z, 0.0f)) {
			int velocitySign = Math::floatSign(m_velocity.z);
			m_velocity.z += dTime * velocitySign * -VELOCITY_DECAY;
			if ((m_velocity.z < 0 && velocitySign > 0) || (m_velocity.z > 0 && velocitySign < 0))
				m_velocity.z = 0.0f;
		}
	}

	resetTranslation();
	translate(0.0f, -2.0f, 0.0f);
	translate(0.0f, 1.0 * m_velocity.y / MAX_VELOCITY, 0.0f);
	translate(-1.0 * Math::floatSign(m_velocity.x) * pow(abs(m_velocity.x), 1.2) / pow(MAX_VELOCITY, 1.2), 0.0f, 0.0f);
	resetRotation();
	rotate(Quaternion(vec3(0.0f, 0.0f, -1.0f), 60.0f * (m_velocity.x / MAX_VELOCITY)));
	rotate(Quaternion(vec3(0.0f, -1.0f, 0.0f), 20.0f * (m_velocity.x / MAX_VELOCITY)));
	rotate(Quaternion(vec3(1.0f, 0.0f, 0.0f), 23.0f * (m_velocity.y / MAX_VELOCITY)));
	m_camera->rotateYaw(-0.5 * m_velocity.x / MAX_VELOCITY);

}

vec3 Vessel::getAcceleration() {
	return m_acceleration;
}

vec3 Vessel::getVelocity() {
	return m_velocity;
}

void Vessel::shake() {
	if (m_shakeCount < 40) {
		lastShake = Quaternion();
		m_shakeCount = 60;
	}
}

void Vessel::draw(DrawType type, Camera& camera, Light& light) {
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
	model = Translate(m_camera->m_position) * m_objectToWorld * m_camera->m_qRotation.generateMatrix();


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
	glUniform1i(uTexture, 0);

	if (m_useTexture) {
		glUniform1i(uEnableTexture, 1);
	} else {
		glUniform1i(uEnableTexture, 0);
	}

	if (m_shakeCount % 20 > 5) {
		glUniform1i(uEnableTexture, 0);
		glUniform4fv(uColor, 1, vec3(1.0, 0.0, 0.0));
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
