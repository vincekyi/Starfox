#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Cube.h"
#include "Types.h"
#include "Tga.h"

#define ESC_KEY 27
#define SPACE_KEY 32

bool g_animate = true;

GLuint g_program;

int g_windowWidth;
int g_windowHeight;

DrawType g_drawType = FILLED;

Camera g_camera;
Light g_light;

Cube* tempShip;
Sphere* tempSphere;
