#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include <string>
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Cube.h"
#include "Types.h"
#include "Tga.h"

#include "ExternalModel.h"

#define ESC_KEY 27
#define SPACE_KEY 32
#define TARGET_FPS 60
#define BLOOPCOUNT 1000

const int UPDATE_DELAY = 1000/TARGET_FPS;
int g_windowWidth;
int g_windowHeight;
float g_FPS;
int g_frameCount;
float g_currentTime = 0;
float g_previousTime = 0;

bool g_animate = true;
bool g_debug = true;


GLuint g_program;

DrawType g_drawType = FILLED;

Camera g_camera;
Camera g_shipCamera;
Light g_light;

Cube* tempShip;
Sphere* tempSphere;

ExternalModel* Vessel;

Sphere* bloop[BLOOPCOUNT];
