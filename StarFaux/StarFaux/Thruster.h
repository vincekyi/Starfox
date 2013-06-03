#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Angel.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "Camera.h"

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const float PI = 3.1415926535f;



const float GRAVITY = 0.0f;
const int NUM_PARTICLES = 1000;
//The interval of time, in seconds, by which the particle engine periodically
//steps.
const float STEP_TIME = 0.01f;
//The length of the sides of the quadrilateral drawn for each particle.
const float PARTICLE_SIZE = 0.05f;

const int NumVertices = 4*NUM_PARTICLES;
point4  points[NumVertices];
vec2    tex_coords[NumVertices];


//Returns a random float from 0 to < 1
float randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}

//Represents a single particle.
struct Particle{
	vec3 pos;
	vec3 velocity;
	vec3 color;
	int timeAliveExplosion; //The amount of time that this particle has been alive.
	int lifespanExplosion;  //The total amount of time that this particle is to live.
	float timeAliveThruster; //The amount of time that this particle has been alive.
	float lifespanThruster;  //The total amount of time that this particle is to live.
};

enum particleType{
	EXPLOSIONS,
	THRUSTERS
}type;

//Rotates the vector by the indicated number of degrees about the specified axis
vec3 rotate(vec3 v, vec3 axis, float degrees) {
	vec3 norm = normalize(axis);
	float radians = degrees * PI / 180;
	float s = sin(radians);
	float c = cos(radians);
	return v * c + axis * dot(norm, v) * (1 - c) + (v*axis) * s;
}

//Returns the position of the particle, after rotating the camera
vec3 adjParticlePos(vec3 pos) {
	return rotate(pos, vec3(1, 0, 0), -30);
}

//Returns whether particle1 is in back of particle2
bool compareParticles(Particle* particle1, Particle* particle2) {
	return adjParticlePos(particle1->pos)[2] <
		adjParticlePos(particle2->pos)[2];
}

class ParticleEngine {
	private:
		GLuint textureId;
		Particle particles[NUM_PARTICLES];
		enum particleType type;
		GLfloat xStrt;
		GLfloat yStrt;
		GLfloat zStrt;

		//The amount of time until the next call to step().
		float timeUntilNextStep;
		//The color of particles that the fountain is currently shooting.  0
		//indicates red, and when it reaches 1, it starts over at red again.  It
		//always lies between 0 and 1.
		float colorTime;
		//The angle at which the fountain is shooting particles, in radians.
		float angle;
		GLuint m_prog;
		
		//Returns the current color of particles produced by the fountain.
		vec3 curColor() {
			vec3 color;
			if (colorTime < 0.166667f) {
				color = vec3(1.0f, colorTime * 6, 0.0f);
			}
			else if (colorTime < 0.333333f) {
				color = vec3((0.333333f - colorTime) * 6, 1.0f, 0.0f);
			}
			else if (colorTime < 0.5f) {
				color = vec3(0.0f, 1.0f, (colorTime - 0.333333f) * 6);
			}
			else if (colorTime < 0.666667f) {
				color = vec3(0.0f, (0.666667f - colorTime) * 6, 1.0f);
			}
			else if (colorTime < 0.833333f) {
				color = vec3((colorTime - 0.666667f) * 6, 0.0f, 1.0f);
			}
			else {
				color = vec3(1.0f, 0.0f, (1.0f - colorTime) * 6);
			}
			
			//Make sure each of the color's components range from 0 to 1
			for(int i = 0; i < 3; i++) {
				if (color[i] < 0) {
					color[i] = 0;
				}
				else if (color[i] > 1) {
					color[i] = 1;
				}
			}
			
			return color;
		}
		
		//Returns the average velocity of particles produced by the fountain.
		vec3 curVelocity() {
			//return vec3(2 * cos(angle), 2.0f, 2 * sin(angle));
			return vec3(0.0f, 1.0f, 1.0f);
		}
		
		//Alters p to be a particle newly produced by the fountain.
		void createParticle(Particle* p) {
			p->pos = vec3(xStrt, yStrt, zStrt);
			p->color = curColor();

			if(type == EXPLOSIONS){
				p->velocity = ((rand() % 100)/ 10.0) * normalize(vec3(float(rand()%200)-100,float(rand()%200)-100,float(rand()%200)-100));
				p->timeAliveExplosion = 0;
				p->lifespanExplosion = 20+(rand()%200);
			}

			if(type == THRUSTERS){
				p->velocity = curVelocity() + vec3(0.5f * randomFloat() - 0.25f,
												0.5f * randomFloat() - 0.25f,
												0.5f * randomFloat() - 0.25f);
				p->timeAliveThruster = 0;
				p->lifespanThruster = randomFloat() + 1;
			}
		}
		
		//Advances the particle fountain by STEP_TIME seconds.
		void step() {
			colorTime += STEP_TIME / 10;
			while (colorTime >= 1) {
				colorTime -= 1;
			}
			
			angle += 0.5f * STEP_TIME;
			while (angle > 2 * PI) {
				angle -= 2 * PI;
			}
			
			for(int i = 0; i < NUM_PARTICLES; i++) {
				Particle* p = particles + i;
				
				p->pos += p->velocity * STEP_TIME;

				if(type == EXPLOSIONS)
					p->timeAliveExplosion += 1;
				
				if(type == THRUSTERS){
					p->velocity += vec3(0.0f, -GRAVITY * STEP_TIME, 0.0f);
					p->timeAliveThruster += STEP_TIME;
					if (p->timeAliveThruster > p->lifespanThruster)
						createParticle(p);
				}
					
			}
		}
	public:
		ParticleEngine(GLuint textureId1, enum particleType pType, GLfloat x, GLfloat y, GLfloat z){
			xStrt = x;
			yStrt = y;
			zStrt = z;

			type = pType;

			textureId = textureId1;
			timeUntilNextStep = 0;
			colorTime = 0;
			angle = 0;
			for(int i = 0; i < NUM_PARTICLES; i++) {
				createParticle(particles + i);
			}

			if(type == THRUSTERS){
				for(int i = 0; i < 5 / STEP_TIME; i++) {
					step();
				}
			}
		}
		
		//Advances the particle fountain by the specified amount of time.
		void advance(float dt) {
			while (dt > 0) {
				if (timeUntilNextStep < dt) {
					dt -= timeUntilNextStep;
					step();
					timeUntilNextStep = STEP_TIME;
				}
				else {
					timeUntilNextStep -= dt;
					dt = 0;
				}
			}
		}

		void createSystem(){
			vector<Particle*> ps;
			for(int i = 0; i < NUM_PARTICLES; i++) {
				ps.push_back(particles + i);
			}
			sort(ps.begin(), ps.end(), compareParticles);

			int z = 1;
			for(int i = 0; i<NUM_PARTICLES; i++){
				Particle* p = ps[i];
				float size = PARTICLE_SIZE / 2;
				vec3 pos = adjParticlePos(p->pos);

				points[4*i] = point4(pos[0] - size, -(pos[1] - size), pos[2], 1.0);
				tex_coords[4*i] = vec2(0, 0);
				points[4*i+1] = point4(pos[0] - size, -(pos[1] + size), pos[2], 1.0);
				tex_coords[4*i+1] = vec2(0, 1);
				points[4*i+2] = point4(pos[0] + size, -(pos[1] + size), pos[2], 1.0);
				tex_coords[4*i+2] = vec2(1, 1);
				points[4*i+3] = point4(pos[0] + size, -(pos[1] - size), pos[2], 1.0);
				tex_coords[4*i+3] = vec2(1, 0);
				//color
				GLuint fColor = glGetUniformLocation( m_prog, "vColor");
				glUniform4fv(fColor, 1, vec4(p->color[0], p->color[1], p->color[2], 1.0));

			}
		}
		void setProgram(GLuint prog){
			m_prog = prog;
		}

		void createNewInstance(GLfloat x, GLfloat y, GLfloat z){
			xStrt = x;
			yStrt = y;
			zStrt = z;
			for(int i = 0; i < NUM_PARTICLES; i++) {
				createParticle(particles + i);
			}
		}

		void texDraw(){
			GLuint vTexCoord = glGetAttribLocation( m_prog, "vTexCoords" );
			glEnableVertexAttribArray( vTexCoord );
		
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

            glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
		}

		void draw(){
			texDraw();
			createSystem();

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
			
			Particle* p = particles;

			for(int i = 0; i<NUM_PARTICLES; i++){

				if(type == EXPLOSIONS){
					if(p->timeAliveExplosion < p->lifespanExplosion) {
						glDrawArrays(GL_QUADS, 4*i, 4);
					}
				}
				
				if(type == THRUSTERS){
					glDrawArrays(GL_QUADS, 4*i, 4);
				}

				p++;
			}
		}

};

class ParticleSystem{
public:
	ParticleSystem(particleType type, vec3 location, GLuint prog, Camera* cam){
		m_type = type;
		m_loc = location;
		m_program = prog;
		initRendering();
		m_cam = cam;
		particleEnginePtr = new ParticleEngine(m_texture, type, m_loc.x, m_loc.y, m_loc.z); 
		particleEnginePtr->setProgram(prog);
	}

	~ParticleSystem(){
		delete particleEnginePtr;
	}

	void initDraw() {

		particleEnginePtr->createSystem();

		// Initialize texture objects
		// Create a vertex array object
		GLuint vao;
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );

		// Create and initialize a buffer object
		GLuint buffer;
		glGenBuffers( 1, &buffer );
		glBindBuffer( GL_ARRAY_BUFFER, buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW );

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(tex_coords), tex_coords);
	
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), sizeof(tex_coords), tex_coords);


		//vertex arrays
		GLuint vPosition = glGetAttribLocation( m_program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	
		//texture
		GLuint texMap = glGetUniformLocation( m_program, "texMap");
		
		glUniform1i(texMap, m_texture);

		//color
		GLuint fColor = glGetUniformLocation( m_program, "vColor");

		// initialize camera shader
		GLuint vModelView = glGetUniformLocation( m_program, "uModelView");
		glUniformMatrix4fv(vModelView, 1, false, m_cam->worldToCamera());
		
		// initialize perspective
		GLuint vPerspective = glGetUniformLocation( m_program, "uProj" );
		glUniformMatrix4fv(vPerspective, 1, false, m_cam->perspective());


		glEnable(GL_DEPTH_TEST);
	}

	void drawScene(Camera* cam) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

		GLuint vModelView = glGetUniformLocation( m_program, "uModelView");
		glUniformMatrix4fv(vModelView, 1, false, cam->worldToCamera());
		
		// initialize perspective
		GLuint vPerspective = glGetUniformLocation( m_program, "uProj" );
		glUniformMatrix4fv(vPerspective, 1, false, cam->perspective());

		particleEnginePtr->draw();
	
		glutSwapBuffers();
	}

	ParticleEngine* getEngine(){
		return particleEnginePtr;
	}
	/*
	void update(int value) {
		_particleEngine->advance(TIMER_MS / 1000.0f);
		glutPostRedisplay();
		glutTimerFunc(TIMER_MS, update, 0);
		
	}*/
private:
	particleType m_type;
	vec3 m_loc;
	GLuint m_program;
	ParticleEngine* particleEnginePtr;
	GLuint m_texture;
	Camera* m_cam;

	void initRendering() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		Image* image = loadBMP("circle.bmp");
		Image* alphaChannel = loadBMP("circlealpha.bmp");
		m_texture = loadAlphaTexture(image, alphaChannel);
		delete image;
		delete alphaChannel;
	}

	//Returns an array indicating pixel data for an RGBA image that is the same as
	//image, but with an alpha channel indicated by the grayscale image alphaChannel
	char* addAlphaChannel(Image* image, Image* alphaChannel) {
		char* pixels = new char[image->width * image->height * 4];
		for(int y = 0; y < image->height; y++) {
			for(int x = 0; x < image->width; x++) {
				for(int j = 0; j < 3; j++) {
					pixels[4 * (y * image->width + x) + j] =
						image->pixels[3 * (y * image->width + x) + j];
				}
				pixels[4 * (y * image->width + x) + 3] =
					alphaChannel->pixels[3 * (y * image->width + x)];
			}
		}
	
		return pixels;
	}

	//Makes the image into a texture, using the specified grayscale image as an
	//alpha channel and returns the id of the texture
	GLuint loadAlphaTexture(Image* image, Image* alphaChannel) {
		char* pixels = addAlphaChannel(image, alphaChannel);
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 image->width, image->height,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 pixels);
	
		delete pixels;
		return textureId;
	}

};








