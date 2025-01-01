#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>

#include <vector>
#include <Windows.h>
#include <assert.h>
#include <iostream>

#include "Utils.h"
#include "Renderer.h"
#include "Obj Parser/wavefront_obj.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "Scene.h"

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

//TWEEK BAR VARIABLES
double g_Scale = 1.0;
//double g_quaternion[4] = {0.0, 0.0, 0.0, 1.0};
double g_TranslateX = 0.0;
double g_TranslateY = 0.0;
double g_TranslateZ = 0.0;
double g_RotateX = 0.0;
double g_RotateY = 0.0;
double g_RotateZ = 0.0; 

bool g_renderBox = true;
bool g_renderNormals = true;

//TWEEK BAR STATE CHANGE FUNCTION DECLARATIONS
void SetRenderBoxState();
void SetRenderNormalsState();


//Create scene
Scene myScene = Scene();
//Create object
Object obj;
//Create renderer
Renderer renderer = Renderer();
//points coordinates
int g_P1x = 200, g_P1y = 200; //P1
int g_P2x = 500, g_P2y = 400; //P2

//line color
unsigned int g_LineColor = 0xffff0000; //red as default

//shape selector
enum ShapeType {
	SHAPE_LINE = 0,
	SHAPE_SQUARE = 1,
	SHAPE_TRIANGLE = 2,
	SHAPE_STAR = 3,
	SHAPE_HEXAGON = 4,
	SHAPE_CIRCLE = 5,
};

// Global Variables
ShapeType g_ShapeSelector = SHAPE_LINE; // Default: Line

// Define the enum for AntTweakBar
TwEnumVal shapeEnum[] = {
	{SHAPE_LINE, "Line"},
	{SHAPE_SQUARE, "Square"},
	{SHAPE_TRIANGLE, "Triangle"},
	{SHAPE_STAR, "Star"},
	{SHAPE_HEXAGON, "Hexagon"},
	{SHAPE_CIRCLE, "Circle"}
};
TwType shapeType;


//obj data type
Wavefront_obj objScene;

void TW_CALL loadOBJModel(void* clientData);
void initScene();
void initGraphics(int argc, char* argv[]);
void drawScene();
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);
void TW_CALL _Scale(void* data);


void TW_CALL SetRenderBoxCallback(void* data);

int main(int argc, char* argv[])
{

	// Initialize openGL, glut, glew
	initGraphics(argc, argv);
	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	//initialize the timer frequency
	QueryPerformanceFrequency(&Frequency);
	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	//send 'glutGetModifers' function pointer to AntTweakBar.
	//required because the GLUT key event functions do not report key modifiers states.
	//TwGLUTModifiersFunc(glutGetModifiers);

	atexit(Terminate);  //called after glutMainLoop ends

	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	shapeType = TwDefineEnum("ShapeType", shapeEnum, 6);

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color
	//the x,y coordinates
	TwAddVarRW(bar, "P1.x", TW_TYPE_INT32, &g_P1x, " min=-1500 max=1500 step=1 keyIncr=x keyDecr=X help='Point 1 x coordinate' ");
	TwAddVarRW(bar, "P1.y", TW_TYPE_INT32, &g_P1y, " min=-1500 max=750 step=1 keyIncr=y keyDecr=Y help='Point 1 y coordinate' ");
	TwAddVarRW(bar, "P2.x", TW_TYPE_INT32, &g_P2x, " min=-1500 max=1500 step=1 keyIncr=a keyDecr=A help='Point 2 x coordinate' ");
	TwAddVarRW(bar, "P2.y", TW_TYPE_INT32, &g_P2y, " min=-1500 max=750 step=1 keyIncr=b keyDecr=B help='Point 2 y coordinate' ");
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");

	TwAddVarRW(bar, "LineColor", TW_TYPE_COLOR32, &g_LineColor, " label='Line Color' help='Change the color of the line.' ");

	//add 'g_Scale' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4D, &g_quaternion, " label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddVarRW(bar, "Shape Selector", shapeType, &g_ShapeSelector,
		" label='Shape Selector' help='Select a shape to display.' ");

	TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");
	TwAddButton(bar, "Apply Scale", _Scale, NULL, " label='object scaling' ");
	//add 'g_Scale' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");
	TwAddVarRW(bar, "TranslateX", TW_TYPE_DOUBLE, &g_TranslateX, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Translate the object in X-axis (0=original size).' ");
	TwAddVarRW(bar, "TranslateY", TW_TYPE_DOUBLE, &g_TranslateY, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Y-axis (0=original size).' ");
	TwAddVarRW(bar, "TranslateZ", TW_TYPE_DOUBLE, &g_TranslateZ, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Z-axis (0=original size).' ");
	TwAddVarRW(bar, "RotateX", TW_TYPE_DOUBLE, &g_RotateX, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around X-axis (0=original size).' ");
	TwAddVarRW(bar, "RotateY", TW_TYPE_DOUBLE, &g_RotateY, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Y-axis (0=original size).' ");
	TwAddVarRW(bar, "RotateZ", TW_TYPE_DOUBLE, &g_RotateZ, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Z-axis (0=original size).' ");

	//add TW option for checkbox to render box by calling the function "toggleRenderBox"
	TwAddVarRW(bar, "Render Box", TW_TYPE_BOOLCPP, &g_renderBox, " label='Render Box' key=b help='Toggle rendering of the bounding box.' ");

	//add TW option for checkbox to render normals by calling the function "toggleNormals"
	TwAddVarRW(bar, "Render Normals", TW_TYPE_BOOLCPP, &g_renderNormals, " label='Render Normals' key=n help='Toggle rendering of the normals.' ");

	//Add TW button to apply box rendering
	TwAddButton(bar, "Apply Box", SetRenderBoxCallback, NULL, " label='Apply Box' help='Apply the box rendering.' ");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}

void TW_CALL SetRenderBoxCallback(void* data) {
	//Toggle the value of g_renderBox
	g_renderBox = !g_renderBox;
	if (myScene._hasObject) {
		renderer.RenderScene(myScene);
		glutPostRedisplay();
	}
}

void callTransform() {

}
void TW_CALL _Scale(void* data) {
	myScene._object->Scale(g_Scale);
	renderer.RenderScene(myScene);
}

void TW_CALL loadOBJModel(void* data)
{
	myScene._hasObject = false;
	myScene._object = nullptr;

	std::wstring str = getOpenFileName();
	
	bool result = objScene.load_file(str);

	if (result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
		return;
	}

	//create object
	obj.ReadFile(objScene);
	//Add object to scene
	myScene.setObject(obj);

	myScene._hasObject = true;

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
}


//do not change this function unless you really know what you are doing!
void initGraphics(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(960, 640);
	glutCreateWindow("Computer Graphics Skeleton using AntTweakBar and freeGlut");
	glutCreateMenu(NULL);

	// Initialize OpenGL
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		assert(0);
		return;
	}
}


void drawLine(int x1, int y1, int x2, int y2, std::vector<Pixel>& pixels) {

	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
	int err = dx - dy, e2;
	Pixel pixel;
	while (true) {
		pixel.x = x1;
		pixel.y = y1;
		pixel.color = g_LineColor;
		pixels.push_back(pixel);
		if (x1 == x2 && y1 == y2) break;

		e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x1 += sx; }
		if (e2 < dx) { err += dx; y1 += sy; }
	}
}

// Generate points for a regular polygon
std::vector<std::pair<int, int>> generatePolygonPoints(int x, int y, int sides, int radius) {
	std::vector<std::pair<int, int>> points;
	double angleStep = 2 * M_PI / sides;

	for (int i = 0; i < sides; ++i) {
		double angle = i * angleStep;
		points.emplace_back(x + static_cast<int>(radius * cos(angle)),
			y - static_cast<int>(radius * sin(angle)));
	}
	return points;
}


//this is just an example of usage for the Renderer::drawPixels function.
//remove these lines and use your own code for drawing the scene.
//it is also a good idea to move the drawScene() function to another file/class
void drawScene()
{
	glm::dmat4x4 model(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glm::vec4 vec(1.0, 1.0, 1.0, 1.0);
	glm::vec4 res = model * vec;
	res = vec * model;


	Renderer renderer;
	std::vector<Pixel> pixels;
	pixels.reserve(100000); //consider changing this if more pixels are colored in the next assignments

	//Variable Declaration
	int y, p;
	Pixel pixel;
	pixel.color = g_LineColor;
	signed char dir;

	int x0 = g_P1x, y0 = g_P1y, x1 = g_P2x, y1 = g_P2y; //variables for manipulation
	//int dx = x1 - x0;
	//int dy = y1 - y0;

	switch (g_ShapeSelector)
	{
	case SHAPE_LINE:
	{
		drawLine(g_P1x, g_P1y, g_P2x, g_P2y, pixels);
		break;
	}
	case SHAPE_SQUARE:
	{
		auto points = generatePolygonPoints(x0, y0, 4, 100);
		for (int i = 0; i < points.size(); ++i) {
			auto next = (i + 1) % points.size();
			drawLine(points[i].first, points[i].second, points[next].first, points[next].second, pixels);
		}
		break;
	}

	case SHAPE_TRIANGLE: {
		auto points = generatePolygonPoints(x0, y0, 3, 100);
		for (size_t i = 0; i < points.size(); ++i) {
			auto next = (i + 1) % points.size();
			drawLine(points[i].first, points[i].second, points[next].first, points[next].second, pixels);
		}
		break;
	}
	case SHAPE_STAR:
	{
		int numPoints = 5; // Five-pointed star
		float angleStep = 2 * M_PI / numPoints;
		int size = 100;
		float innerRadius = size / 2.5f; // Smaller radius for inner points

		std::vector<std::pair<int, int>> points;

		// Generate points for the star
		for (int i = 0; i < 2 * numPoints; i++) {
			float radius = (i % 2 == 0) ? size : innerRadius;
			float angle = i * angleStep / 2.0f;
			int px = x0 + static_cast<int>(radius * cos(angle));
			int py = y0 - static_cast<int>(radius * sin(angle));
			points.emplace_back(px, py);
		}

		// Draw lines between consecutive points
		for (size_t i = 0; i < points.size(); i++) {
			int nextIndex = (i + 1) % points.size();
			drawLine(points[i].first, points[i].second,
				points[nextIndex].first, points[nextIndex].second, pixels);
		}
		break;
	}
	case SHAPE_HEXAGON:
	{
		auto points = generatePolygonPoints(x0, y0, 6, 100);
		for (size_t i = 0; i < points.size(); ++i) {
			auto next = (i + 1) % points.size();
			drawLine(points[i].first, points[i].second, points[next].first, points[next].second, pixels);
		}
		break;
	}
	case SHAPE_CIRCLE:
	{
		int radius = 100;
		int x = radius, y = 0;
		int radiusError = 1 - x;

		while (x >= y) {
			pixel.x = x0 + x;
			pixel.y = y0 + y;
			pixels.push_back(pixel);
			pixel.x = x0 + x;
			pixel.y = y0 - y;
			pixels.push_back(pixel);
			pixel.x = x0 + y;
			pixel.y = y0 + x;
			pixels.push_back(pixel);
			pixel.x = x0 + y;
			pixel.y = y0 - x;
			pixels.push_back(pixel);

			pixel.x = x0 - x;
			pixel.y = y0 + y;
			pixels.push_back(pixel);
			pixel.x = x0 - x;
			pixel.y = y0 - y;
			pixels.push_back(pixel);
			pixel.x = x0 - y;
			pixel.y = y0 + x;
			pixels.push_back(pixel);
			pixel.x = x0 - y;
			pixel.y = y0 - x;
			pixels.push_back(pixel);

			y++;
			if (radiusError < 0) {
				radiusError += 2 * y + 1;
			}
			else {
				x--;
				radiusError += 2 * (y - x + 1);
			}
		}

		break;
	}
	default:
		break;
	}
	renderer.drawPixels(pixels);
}


//this will make sure that integer coordinates are mapped exactly to corresponding pixels on screen
void glUseScreenCoordinates(int width, int height)
{
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Callback function called by GLUT to render screen
void Display()
{
	//  	static int counter = 0;
	//  	std::cout << "C: " << counter << std::endl;
	//  	counter++;

	glClearColor(0, 0, 0, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//time measuring - don't delete
	QueryPerformanceCounter(&StartingTime);

	
	//Changes States
	SetRenderBoxState();
	SetRenderNormalsState();

	//draw the scene
	if (myScene._hasObject)
		renderer.RenderScene(myScene);
	else {
		drawScene();
	}
	

	//time measuring - don't delete
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	// Draw tweak bars
	TwDraw();

	//swap back and front frame buffers
	glutSwapBuffers();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	glUseScreenCoordinates(width, height);

	//////////////////////////////////////
	///////add your reshape code here/////////////

	//Update the aspect ratio for camera projection matrix
	myScene._camera.UpdateAspectRatio(width/height);
	//Update the viewport matrix
	renderer.CalculateViewPortMatrix(width, height);

	//////////////////////////////////////

	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
	glutPostRedisplay();
}



void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
}


void Keyboard(unsigned char k, int x, int y)
{
	TwEventKeyboardGLUT(k, x, y);
	glutPostRedisplay();
}


void Special(int k, int x, int y)
{
	TwEventSpecialGLUT(k, x, y);
	glutPostRedisplay();
}


// Function called at exit
void Terminate(void)
{
	TwTerminate();
}

//State Control Functions
void SetRenderBoxState() {
	renderer._enablePrintBox = g_renderBox;
}

void SetRenderNormalsState() {
	renderer._enablePrintNormals = g_renderNormals;
}