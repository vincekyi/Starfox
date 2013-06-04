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
#include "ExternalModel.h"
#include "Vessel.h"
#include "Types.h"
#include "Tga.h"

#include "Line.h"

#include "Sound.h"

#include "AL/al.h" 
#include "AL/alc.h" 
#include "AL/alut.h"
#include "Background.h"


#define ESC_KEY 27
#define SPACE_KEY 32
#define TARGET_FPS 100
#define BLOOPCOUNT 300
const int NUM_PARENT_ASTEROIDS = 3;

#define LIGHTSOURCECOUNT 2
#define MUSICGAIN 0.5f
#define SPEED_LINE_COUNT 10

const int SPEED_LINE_SPEED = 2.0;

#define ACCEL 0.05f

vec3 g_bulletV(0.0f);

bool g_keyPress[40];
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
Light* g_light;
Vessel* g_vessel;
Background* starField;

Cube* tempShip;
Cube* greenStar;

Cube* xhair1;
Cube* xhair2;

ExternalModel *bloop[BLOOPCOUNT];

ExternalModel *gMamaAsteroid;
ExternalModel *gPapaAsteroid;
ExternalModel *gUncleAsteroid;

Line *speedLine[SPEED_LINE_COUNT];

Sound* g_sound;
Sound* g_music;
