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

void calculateFPS()
{
    g_frameCount++;
    g_currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = g_currentTime - g_previousTime;
    if(timeInterval > 100)
    {
        g_FPS = g_frameCount / (timeInterval / 1000.0f);
        g_previousTime = g_currentTime;
        g_frameCount = 0;
    }
}

void debugDisplay() {
	// Position
	char coords[100];
	sprintf(coords, "x: %.2f\ny: %.2f\nz: %.2f\n", g_camera.m_position.x, g_camera.m_position.y - 2.0, g_camera.m_position.z - 10.0);
	glRasterPos2f(-0.97f, 0.90f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) coords);

	// Velocity
	char v[100];
	vec3 vel = g_vessel->getVelocity();
	sprintf(v, "V.x: %.2f\nV.y: %.2f\nV.z: %.2f\n", vel.x, vel.y, vel.z);
	glRasterPos2f(-0.97f, 0.650f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) v);

	// Acceleration
	char a[100];
	vec3 accel = g_vessel->getAcceleration();
	sprintf(a, "A.x: %.3f\nA.y: %.3f\nA.z: %.3f\n", accel.x, accel.y, accel.z);
	glRasterPos2f(-0.97f, 0.40f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) a);

	char fps[30];
	sprintf(fps, "FPS: %.2f", g_FPS);
	glRasterPos2f(0.82f, 0.90f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) fps);

	char health[20];
	sprintf(health, "Health: %i", g_vessel->m_health);
	glRasterPos2f(-0.97f, -0.9f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) health);
}

void handleKeyDown() {
	for (int i = 0; i < 40; ++i) {
		if (g_keyPress[i]) {
			switch (i) {
				//case KEY_W: g_vessel->setAccelerationZ(-0.05f); break;
				case KEY_A: g_vessel->setAccelerationX(-0.5*ACCEL); break;
				//case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationZ(0.05f); break;
				case KEY_D: if (!g_keyPress[KEY_A]) g_vessel->setAccelerationX(0.5*ACCEL); break;
				case KEY_W: g_vessel->setAccelerationY(ACCEL); break;
				case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationY(-ACCEL); break;
				case KEY_E: g_vessel->setAccelerationZ(-ACCEL); break;
				case KEY_R: g_vessel->setAccelerationZ(ACCEL); break;
				case KEY_UP: g_camera.rotatePitch(2.0f); break;
				case KEY_DOWN: g_camera.rotatePitch(-2.5f); break;
				case KEY_LEFT: g_camera.rotateYaw(2.0f); break;
				case KEY_RIGHT: g_camera.rotateYaw(-2.5f); break;
			}
		} else {
			switch (i) {
				//case KEY_W: if (!g_keyPress[KEY_S]) g_vessel->setAccelerationZ(0.0f); break;
				case KEY_A: if (!g_keyPress[KEY_D]) g_vessel->setAccelerationX(0.0f); break;
				//case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationZ(0.0f); break;
				case KEY_D: if (!g_keyPress[KEY_A]) g_vessel->setAccelerationX(0.0f); break;
				case KEY_W: if (!g_keyPress[KEY_S]) g_vessel->setAccelerationY(0.0f); break;
				case KEY_S: if (!g_keyPress[KEY_W]) g_vessel->setAccelerationY(0.0f); break;
				case KEY_E: if (!g_keyPress[KEY_R]) g_vessel->setAccelerationZ(0.0f); break;
				case KEY_R: if (!g_keyPress[KEY_E]) g_vessel->setAccelerationZ(0.0f); break;
			}
		}
	}
}

// Called when the window needs to be redrawn.
void callbackDisplay()
{
	handleKeyDown();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_camera.update();
	g_shipCamera.update();
	g_vessel->updateMovement();

	g_light.m_position = vec3(0.0, 0.0, 0.0);
	g_light.m_lightAmbient = vec4(1.0, 1.0, 1.0, 1.0);
	g_light.m_lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
	g_light.m_lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

	GLuint fogColor = glGetUniformLocation(g_program, "uFogColor");
	GLuint fogMinDist = glGetUniformLocation(g_program, "uFogMinDist");
	GLuint fogMaxDist = glGetUniformLocation(g_program, "uFogMaxDist");
	glUniform1f(fogMinDist, 1500.0f);
	glUniform1f(fogMaxDist, 1700.0f);
	glUniform4fv(fogColor, 1, vec4(0.0, 0.0, 0.0, 0.0));
	
	tempShip->draw(g_drawType, g_camera, g_light);
	if (g_vessel->m_shape->checkCollision(tempShip->m_shape)) {
		std::cout << g_vessel->m_shape->m_center << std::endl;
		std::cout << "BOOP1" << glutGet(GLUT_ELAPSED_TIME) <<  std::endl;
		g_vessel->shake();
	}
	for (int i = 0; i < BLOOPCOUNT; ++i) {
		bloop[i]->draw(g_drawType, g_camera, g_light);
		if (g_vessel->m_shape->checkCollision(bloop[i]->m_shape)) {
			std::cout << "BOOP" << glutGet(GLUT_ELAPSED_TIME) << std::endl;
			g_vessel->shake();
		}
	}
	g_light.m_position = g_shipCamera.m_position;
	//tempSphere->draw(g_drawType, g_shipCamera, g_light);

	g_vessel->draw(g_drawType, g_shipCamera, g_light);
	//g_vessel->draw(g_drawType, g_camera, g_light);

	if (g_debug) 
		debugDisplay();


	calculateFPS();
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
		case 'w': g_keyPress[KEY_W] = true; break;
		case 'a': g_keyPress[KEY_A] = true; break;
		case 's': g_keyPress[KEY_S] = true; break;
		case 'd': g_keyPress[KEY_D] = true; break;
		case 'r': g_keyPress[KEY_R] = true; break;
		case 'f': g_keyPress[KEY_F] = true; break;
		case 'e': g_keyPress[KEY_E] = true; break;
		case '1': g_drawType = (g_drawType == FILLED ? MESH : FILLED); break;
		case '0': g_debug = !g_debug; break;
		case 'p': g_animate = !g_animate; break;
	}
}

void callbackKeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w': g_keyPress[KEY_W] = false; break;
		case 'a': g_keyPress[KEY_A] = false; break;
		case 's': g_keyPress[KEY_S] = false; break;
		case 'd': g_keyPress[KEY_D] = false; break;
		case 'r': g_keyPress[KEY_R] = false; break;
		case 'f': g_keyPress[KEY_F] = false; break;
		case 'e': g_keyPress[KEY_E] = false; break;
	}
}

void callbackSpecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: g_keyPress[KEY_UP] = true; break;
		case GLUT_KEY_DOWN: g_keyPress[KEY_DOWN] = true; break;
		case GLUT_KEY_LEFT: g_keyPress[KEY_LEFT] = true; break;
		case GLUT_KEY_RIGHT: g_keyPress[KEY_RIGHT] = true; break;
	}
}

void callbackSpecialUp(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP: g_keyPress[KEY_UP] = false; break;
		case GLUT_KEY_DOWN: g_keyPress[KEY_DOWN] = false; break;
		case GLUT_KEY_LEFT: g_keyPress[KEY_LEFT] = false; break;
		case GLUT_KEY_RIGHT: g_keyPress[KEY_RIGHT] = false; break;
	}
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
	if (g_animate) {
		glutPostRedisplay();
	}
	glutTimerFunc(UPDATE_DELAY, callbackTimer, 0);
}

void initCallbacks()
{
	glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutKeyboardFunc(callbackKeyboard);
	glutKeyboardUpFunc(callbackKeyboardUp);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	//glutIdleFunc(callbackIdle);
	glutTimerFunc(UPDATE_DELAY, callbackTimer, 0);
	glutSpecialFunc(callbackSpecial);
	glutSpecialUpFunc(callbackSpecialUp);
}

void init() {
	for (int i = 0; i < 40; ++i) {
		g_keyPress[i] = false;
	}
	BoundingBox* bb;
	BoundingSphere* bs;

	glEnable(GL_DEPTH_TEST);

	g_program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(g_program); 

	g_camera.init(45.0, (double) g_windowWidth/g_windowHeight, 0.1, 4000.0);
	g_camera.translate(vec3(0.0, 0.0, 1500.0));
	g_shipCamera.init(45.0, (double) g_windowWidth/g_windowHeight, 0.1, 250.0);
	g_shipCamera.translate(vec3(0.0, 2.0, 10.0));
	g_shipCamera.rotatePitch(0.0f);

	tempShip = new Cube(g_program, FLAT);
	tempShip->setupLighting(20.0, vec4(0.8, 0.8, 0.8, 1.0), vec4(0.8, 0.8, 0.8, 1.0), vec4(0.8, 0.8, 0.8, 1.0));
	tempShip->initDraw();
	tempShip->scale(30.0);
	bb = new BoundingBox();
	bb->setHalfWidths(15.0, 15.0, 15.0);
	tempShip->m_shape = bb;
	
	g_vessel = new Vessel(g_program, &g_camera, "./models/ship/", FLAT);
	g_vessel->loadModel("ship.obj", true);
	g_vessel->setupLighting();
	g_vessel->setupTexture(TRILINEAR, REPEAT);
	g_vessel->initDraw();
	g_vessel->scale(0.5);
	bb = new BoundingBox();
	bb->setHalfWidths(2.0, 1.0, 4.5);
	bb->setCenter(vec3(0.0, 0.0, 1500.0));
	g_vessel->m_shape = bb;

	float start = 280.0f;
	for (int i = 0; i < BLOOPCOUNT; ++i) {
		bloop[i] = new Sphere(g_program, rand() % 3, FLAT);
		float sc = 10.0f + (rand() % 200 / 10.0f);
		sc = 30.0f;
		bloop[i]->scale(sc);
		bs = new BoundingSphere();
		bs->m_radius = sc;
		bloop[i]->m_shape = bs;
		bloop[i]->setupLighting(20.0, vec4(0.55, 0.27, 0.07, 1.0), vec4(0.55, 0.27, 0.07, 1.0), vec4(0.55, 0.27, 0.07, 1.0));
		//bloop[i]->setupLighting(FLAT, 20.0, 0.2 * vec4(1.0, 0.3, 0.0, 1.0), 0.5 * vec4(1.0, 0.3, 0.0, 1.0), 0.5 * vec4(1.0, 1.0, 1.0, 1.0));
		bloop[i]->initDraw();
		//bloop[i]->translate(rand() % 4000 - 2000, rand() % 4000 - 2000, rand() % 4000 - 2000);

		start -= 20.0f;
	}

	g_vessel->setAccelerationZ(-0.01);
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
