#include "main.h"

void initGlut(int& argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	g_windowWidth = 1280;
	g_windowHeight = 720;
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	glutCreateWindow("Star Faux");
}

// Called when the window needs to be redrawn.
void callbackDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_camera.update();
	g_light.m_position = g_camera.m_position;
	tempShip->draw(g_drawType, g_camera, g_light);
	tempSphere->draw(g_drawType, g_camera, g_light);
	glutSwapBuffers();
}

// Called when the window is resized.
void callbackReshape(int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;
	g_camera.m_aspect = (double) width/height;
	glViewport(0, 0, width, height);
}

// Called when a key is pressed. x, y is the current mouse position.
void callbackKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case ESC_KEY:
		case 'q':
		case 'Q': exit(0); break;
		case 'w': g_camera.translate(0.0, 0.0, -0.5); break;
		case 'a': g_camera.translate(-0.5, 0.0, 0.0); break;
		case 's': g_camera.translate(0.0, 0.0, 0.5); break;
		case 'd': g_camera.translate(0.5, 0.0, 0.0); break;
		case 'r': g_camera.translate(0.0, 0.5, 0.0); break;
		case 'f': g_camera.translate(0.0, -0.5, 0.0); break;
		case '1': g_drawType = (g_drawType == FILLED ? MESH : FILLED); break;
		case 'p': g_animate = !g_animate; break;
	} 
	glutPostRedisplay();
}

void callbackSpecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: g_camera.rotatePitch(1.0); break;
		case GLUT_KEY_DOWN: g_camera.rotatePitch(-1.0); break;
		case GLUT_KEY_LEFT: g_camera.rotateYaw(1.0); break;
		case GLUT_KEY_RIGHT: g_camera.rotateYaw(-1.0); break;
	}
	glutPostRedisplay();
}

// Called when the system is idle. Can be called many times per frame.
void callbackIdle()
{
}

// Called when a mouse button is pressed or released
void callbackMouse(int button, int state, int x, int y)
{
}

// Called when the mouse is moved with a button pressed
void callbackMotion(int x, int y)
{
}

// Called when the mouse is moved with no buttons pressed
void callbackPassiveMotion(int x, int y)
{
}

// Called when the timer expires
void callbackTimer(int)
{
}

void initCallbacks()
{
	glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutKeyboardFunc(callbackKeyboard);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	glutIdleFunc(callbackIdle);
	glutTimerFunc(1, callbackTimer, 0);
	glutSpecialFunc(callbackSpecial);
}

void init() {
	glEnable(GL_DEPTH_TEST);
	g_program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(g_program);

	g_camera.init(45.0, (double) g_windowWidth/g_windowHeight, 0.1, 500.0);
	g_camera.translate(vec3(2.5, 0.0, 10.0));

	tempShip = new Cube(g_program, vec4(0.8, 0.8, 0.8, 1.0));
	tempShip->setupLighting(FLAT, 20.0, vec4(1.0, 1.0, 1.0, 1.0));
	tempShip->initDraw();

	tempSphere = new Sphere(g_program, 4, vec4(1.0, 0.0, 0.0, 1.0));
	tempSphere->setupLighting(GOURAUD, 20.0, vec4(1.0, 1.0, 1.0, 1.0));
	tempSphere->initDraw();
	tempSphere->m_objectToWorld = Translate(5.0, 0.0, 0.0);

	glClearColor( 0.0, 0.0, 0.0, 0.0 ); // black background
}

int main(int argc, char** argv)
{
	initGlut(argc, argv);
	initCallbacks();
	glewInit();
	init();
	glutMainLoop();
	return 0;
}
