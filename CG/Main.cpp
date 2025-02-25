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
double g_quaternion[4] = { 0.0, 0.0, 0.0, 1.0 };

//Transformations
double g_Scale = 5.0;
double g_TranslateX = 0.0;
double g_TranslateY = 0.0;
double g_TranslateZ = 0.0;
double g_RotateX = 0.0;
double g_RotateY = 0.0;
double g_RotateZ = 0.0;
double Wg_Scale = 5.0;
double Wg_TranslateX = 0.0;
double Wg_TranslateY = 0.0;
double Wg_TranslateZ = 0.0;
double Wg_RotateX = 0.0;
double Wg_RotateY = 0.0;
double Wg_RotateZ = 0.0;
//Render Options
bool g_renderWorldAxis = true;
bool g_renderBox = true;
bool g_renderPointNormals = false;
bool g_renderFaceNormals = false;
bool g_renderObjectAxis = true;
unsigned int g_ObjectColor = 0xfffffffff; //white as default

//TWEEK BAR FUNCTION DECLARATIONS
	// Object Transformations
void TW_CALL Scale(void* data);
void TW_CALL Translate(void* data);
void TW_CALL Rotate(void* data);
//World Transformations
void TW_CALL ScaleWorld(void* data);
void TW_CALL TranslateWorld(void* data);
void TW_CALL RotateWorld(void* data);
//Camera Update
void TW_CALL ViewMatrixUpdate(void* data);
void TW_CALL CameraLookAt(void* data);
//Projection Update
void TW_CALL ProjectionMatrixUpdateFOV(void* data);
void TW_CALL ProjectionMatrixUpdateRightTop(void* data);
//Render Options
void TW_CALL SetRenderBoxState(void* data);
void TW_CALL ToggleRenderFaceNormals(void* data);
void TW_CALL ToggleRenderPointNormals(void* data);
void TW_CALL SetRenderObjectAxisState(void* data);
void TW_CALL SetRenderWorldAxisState(void* data);
void TW_CALL SetObjectColor(void* data);

//Viewport
//Quaternion
void TW_CALL Rotateq(void* data);
//Create scene
Scene myScene = Scene();
//Create renderer
Renderer renderer = Renderer();
//points coordinates
int g_P1x = 200, g_P1y = 200; //P1
int g_P2x = 500, g_P2y = 400; //P2

//line color
unsigned int g_LineColor = 0xfffffffff; //red as default

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

TwEnumVal lightEnum[] = {
	{static_cast<int>(LightType::Point), "Point"},
	{static_cast<int>(LightType::Directional), "Directional"},
	{static_cast<int>(LightType::Ambient), "Ambient"}
};
TwType lightType;

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
	lightType = TwDefineEnum("LightType", lightEnum, 3);
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color

	//Shape Drawing group
		//the x,y coordinates
	TwAddVarRW(bar, "P1.x", TW_TYPE_INT32, &g_P1x, " min=-1500 max=1500 step=1 keyIncr=x keyDecr=X help='Point 1 x coordinate' group='Shape Drawing' ");
	TwAddVarRW(bar, "P1.y", TW_TYPE_INT32, &g_P1y, " min=-1500 max=750 step=1 keyIncr=y keyDecr=Y help='Point 1 y coordinate' group='Shape Drawing' ");
	TwAddVarRW(bar, "P2.x", TW_TYPE_INT32, &g_P2x, " min=-1500 max=1500 step=1 keyIncr=a keyDecr=A help='Point 2 x coordinate' group='Shape Drawing' ");
	TwAddVarRW(bar, "P2.y", TW_TYPE_INT32, &g_P2y, " min=-1500 max=750 step=1 keyIncr=b keyDecr=B help='Point 2 y coordinate' group='Shape Drawing' ");
	//line color
	TwAddVarRW(bar, "LineColor", TW_TYPE_COLOR32, &g_LineColor, " label='Line Color' help='Change the color of the line.' group='Shape Drawing'");
	//shape selector
	TwAddVarRW(bar, "Shape Selector", shapeType, &g_ShapeSelector, " label='Shape Selector' help='Select a shape to display.' group='Shape Drawing'");
	//time to draw the shape
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds' group='Shape Drawing'");
	//light selector
	TwAddVarRW(bar, "Light 1 Source Selector", lightType, &myScene._lightSource1._type, " label='Light Selector' help='Select a light to display.' group='Lighting'");

	//TwAddVarRW(bar, "lightdir1", TW_TYPE_DIR3D, &obj.light.light1, "help='RGB colors' group='Lighting' ");
	TwAddVarRW(bar, "lightdir1", TW_TYPE_DIR3D, &myScene._lightSource1._direction, "help='RGB colors' group='Lighting' ");

	TwAddVarRW(bar, "Light 2 Source Selector", lightType, &myScene._lightSourceOptional._type, " label='Light Selector' help='Select a light to display.' group='Lighting'");
	//TwAddVarRW(bar, "lightdir2", TW_TYPE_DIR3D, &obj.light.light2, "help='RGB colors' group='Light2' ");
	TwAddVarRW(bar, "Enable Light 2", TW_TYPE_BOOLCPP, &myScene._lightSourceOptional._enabled, " label='Enable Light 2' key=n help='Enable optional light.' group = 'Lighting'");
	//Load OBJ file
	TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");

	//Object color
	TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR32, &g_ObjectColor, " label='Object Color' help='Change the color of the object.'");
	TwAddButton(bar, "Change Object Color", SetObjectColor, NULL, " label='Change Color'");

	//Object Transformations group
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=20 step=0.01 keyIncr=s keyDecr=S help='Scale the object (5=original size).' group = 'Object Transformations' ");
	TwAddButton(bar, "Apply Object Scale", Scale, NULL, " label='Scale Object' group = 'Object Transformations'");

	TwAddVarRW(bar, "TranslateX", TW_TYPE_DOUBLE, &g_TranslateX, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in X-axis (0=original size).' group = 'Object Transformations'");
	TwAddVarRW(bar, "TranslateY", TW_TYPE_DOUBLE, &g_TranslateY, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Y-axis (0=original size).' group = 'Object Transformations'");
	TwAddVarRW(bar, "TranslateZ", TW_TYPE_DOUBLE, &g_TranslateZ, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Z-axis (0=original size).' group = 'Object Transformations'");
	TwAddButton(bar, "Apply Object Translate", Translate, NULL, " label='Translate' group = 'Object Transformations'");

	TwAddVarRW(bar, "RotateX", TW_TYPE_DOUBLE, &g_RotateX, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around X-axis (0=original size).' group = 'Object Transformations'");
	TwAddVarRW(bar, "RotateY", TW_TYPE_DOUBLE, &g_RotateY, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Y-axis (0=original size).' group = 'Object Transformations'");
	TwAddVarRW(bar, "RotateZ", TW_TYPE_DOUBLE, &g_RotateZ, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Z-axis (0=original size).' group = 'Object Transformations'");
	TwAddButton(bar, "Apply Object Rotation", Rotate, NULL, " label='Rotate Object' group = 'Object Transformations'");

	//World Transformations group
	TwAddVarRW(bar, "_Scale", TW_TYPE_DOUBLE, &Wg_Scale, " min=0.01 max=20 step=0.01 keyIncr=s keyDecr=S help='Scale the object (5=original size).' group = 'World Transformations' ");
	TwAddButton(bar, "Apply Global Scale", ScaleWorld, NULL, " label='Scale World' group = 'World Transformations'");

	TwAddVarRW(bar, "_TranslateX", TW_TYPE_DOUBLE, &Wg_TranslateX, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in X-axis (0=original size).' group = 'World Transformations'");
	TwAddVarRW(bar, "_TranslateY", TW_TYPE_DOUBLE, &Wg_TranslateY, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Y-axis (0=original size).' group = 'World Transformations'");
	TwAddVarRW(bar, "_TranslateZ", TW_TYPE_DOUBLE, &Wg_TranslateZ, " min=-50 max=50 step=0.01 keyIncr=s keyDecr=S help='Translate the object in Z-axis (0=original size).' group = 'World Transformations'");
	TwAddButton(bar, "Apply Global Translate", TranslateWorld, NULL, " label='Translate' group = 'World Transformations'");

	TwAddVarRW(bar, "_RotateX", TW_TYPE_DOUBLE, &Wg_RotateX, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around X-axis (0=original size).' group = 'World Transformations'");
	TwAddVarRW(bar, "_RotateY", TW_TYPE_DOUBLE, &Wg_RotateY, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Y-axis (0=original size).' group = 'World Transformations'");
	TwAddVarRW(bar, "_RotateZ", TW_TYPE_DOUBLE, &Wg_RotateZ, " min=0.0 max=360 step=1 keyIncr=s keyDecr=S help='Rotate the object around Z-axis (0=original size).' group = 'World Transformations'");
	TwAddButton(bar, "Apply Global Rotation", RotateWorld, NULL, " label='Rotate World' group = 'World Transformations'");


	//Camera group
		//camera position variable
	TwAddVarRW(bar, "Camera X pos.", TW_TYPE_FLOAT, &myScene._camera._eye.x, " label='x pos. (cam.)' help='Change the camera position.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Y pos.", TW_TYPE_FLOAT, &myScene._camera._eye.y, " label='y pos. (cam.)' help='Change the camera position.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Z pos.", TW_TYPE_FLOAT, &myScene._camera._eye.z, " label='z pos. (cam.)' help='Change the camera position.' group = 'Camera'");
	//camera target variable
	TwAddVarRW(bar, "Camera Target X", TW_TYPE_FLOAT, &myScene._camera._target.x, " label='x pos. (tar.)' help='Change the camera target.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Target Y", TW_TYPE_FLOAT, &myScene._camera._target.y, " label='y pos. (tar.)' help='Change the camera target.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Target Z", TW_TYPE_FLOAT, &myScene._camera._target.z, " label='z pos. (tar.)' help='Change the camera target.' group = 'Camera'");
	//camera up variable
	TwAddVarRW(bar, "Camera Up X", TW_TYPE_FLOAT, &myScene._camera._up.x, " label='x pos. (up)' help='Change the camera up.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Up Y", TW_TYPE_FLOAT, &myScene._camera._up.y, " label='y pos. (up)' help='Change the camera up.' group = 'Camera'");
	TwAddVarRW(bar, "Camera Up Z", TW_TYPE_FLOAT, &myScene._camera._up.z, " label='z pos. (up)' help='Change the camera up.' group = 'Camera'");
	//camera configuration - view matrix
	TwAddButton(bar, "Update Camera", ViewMatrixUpdate, NULL, " label='Update Camera' help='Update the camera view matrix.' group = 'Camera'");
	//add Lookat function
	TwAddButton(bar, "LookAt", CameraLookAt, NULL, " label='Look At' help='Update the camera view matrix.' group = 'Camera'");

	//Projection group
		//projection variables
	TwAddVarRW(bar, "Camera Near", TW_TYPE_FLOAT, &myScene._camera._near, " min=0.1 max=1000 step=0.1 keyIncr=n keyDecr=N help='Change the near clipping plane.' group = 'Projection'");
	TwAddVarRW(bar, "Camera Far", TW_TYPE_FLOAT, &myScene._camera._far, " min=0.1 max=1000 step=0.1 keyIncr=f keyDecr=F help='Change the far clipping plane.' group = 'Projection'");
	TwAddVarRW(bar, "Camera Right", TW_TYPE_FLOAT, &myScene._camera._right, " min=0.1 max=1000 step=0.1 keyIncr=r keyDecr=R help='Change the right of the frustum.' group = 'Projection'");
	TwAddVarRW(bar, "Camera Top", TW_TYPE_FLOAT, &myScene._camera._top, " min=0.1 max=1000 step=0.1 keyIncr=t keyDecr=T help='Change the top of the frustum.' group = 'Projection'");
	TwAddVarRW(bar, "Camera FOV", TW_TYPE_FLOAT, &myScene._camera._fov, " min=0.1 max=1000 step=0.1 keyIncr=v keyDecr=V help='Change the field of view.' group = 'Projection'");
	//projection update
		//based on right and top
	TwAddButton(bar, "Update Camera (right-top)", ProjectionMatrixUpdateRightTop, NULL, " label='Update Right-Top' help='Update the camera perspective.' group = 'Projection'");
	//based on FOV
	TwAddButton(bar, "Update Camera (FOV)", ProjectionMatrixUpdateFOV, NULL, " label='Update FOV' help='Update the camera perspective.' group = 'Projection'");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4D, &g_quaternion, " label='Object rotation' opened=true help='Change the object orientation.' ");
	//TwAddButton(bar, "Apply Object Rotation (quaternion)", Rotateq, NULL, " label='Rotate Object (quaternion)' group = 'ObjRotation'");
	//Render Options (Render group)
	TwAddVarRW(bar, "Render Box", TW_TYPE_BOOLCPP, &g_renderBox, " label='Bounding Box' key=b help='Toggle rendering of the bounding box.' group = 'Render Options'");
	TwAddButton(bar, "Apply Box", SetRenderBoxState, NULL, " label='Apply Box' help='Apply the box rendering.' group = 'Render Options'");

	TwAddVarRW(bar, "Render Face Normals", TW_TYPE_BOOLCPP, &g_renderFaceNormals, " label='Face Normals' key=n help='Toggle rendering of the normals.' group = 'Render Options'");
	TwAddButton(bar, "Apply Face Normals", ToggleRenderFaceNormals, NULL, " label='Apply Normals' help='Apply the normals rendering.' group = 'Render Options'");

	TwAddVarRW(bar, "Render Point Normals", TW_TYPE_BOOLCPP, &g_renderPointNormals, " label='Vertex Normals' key=n help='Toggle rendering of the normals.' group = 'Render Options'");
	TwAddButton(bar, "Apply Point Normals", ToggleRenderPointNormals, NULL, " label='Apply Normals' help='Apply the normals rendering.' group = 'Render Options'");


	TwAddVarRW(bar, "Render Object Axis", TW_TYPE_BOOLCPP, &g_renderObjectAxis, " label='Object Axis' key=n help='Toggle rendering of the object axis.' group = 'Render Options'");
	TwAddButton(bar, "Apply Object Axis", SetRenderObjectAxisState, NULL, " label='Apply Object Axis' help='Apply the object axis rendering.' group = 'Render Options'");
	TwAddVarRW(bar, "Render World Axis", TW_TYPE_BOOLCPP, &g_renderWorldAxis, " label='World Axis' key=n help='Toggle rendering of the world axis.' group = 'Render Options'");
	TwAddButton(bar, "Apply World Axis", SetRenderWorldAxisState, NULL, " label='Apply World Axis' help='Apply the world axis rendering.' group = 'Render Options'");

	//Default group view settings
	TwDefine(" TweakBar/'Shape Drawing' opened=true ");
	TwDefine(" TweakBar/'Object Transformations' opened=false ");
	TwDefine(" TweakBar/'World Transformations' opened=false ");
	TwDefine(" TweakBar/Camera opened=false ");
	TwDefine(" TweakBar/Projection opened=false ");
	TwDefine(" TweakBar/'Render Options' opened=false ");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}

//Callback Functions
void TW_CALL loadOBJModel(void* data)
{
	//Temporary object to store the obj file
	Wavefront_obj objScene;

	std::wstring str = getOpenFileName();

	bool result = objScene.load_file(str);

	if (result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
		//Reset the scene
		myScene.Reset();
		//Add object to scene
		myScene.setObject(objScene);
		//Enable object rendering
		renderer._objectChanged = true;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
		return;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
}

void TW_CALL Scale(void* data) {
	myScene._object.Scale(g_Scale, ObjectTransform);
	renderer._objectChanged = true;
}
void TW_CALL Translate(void* data) {
	myScene._object.Translate(g_TranslateX, g_TranslateY, g_TranslateZ, ObjectTransform);
	renderer._objectChanged = true;
}
void TW_CALL Rotate(void* data) {
	myScene._object.Rotate(g_RotateX, g_RotateY, g_RotateZ, ObjectTransform);
	renderer._objectChanged = true;
}

void TW_CALL Rotateq(void* data) {
	myScene._object.Rotate(g_quaternion[0] * 360, g_quaternion[1] * 360, g_quaternion[2] * 360, ObjectTransform);
	myScene._object.Scale(g_quaternion[4] * 4, ObjectTransform);
	renderer._objectChanged = true;
}

void TW_CALL ScaleWorld(void* data) {
	myScene._object.Scale(Wg_Scale, WorldTransform);
	renderer._objectChanged = true;
}
void TW_CALL RotateWorld(void* data) {
	myScene._object.Rotate(Wg_RotateX, Wg_RotateY, Wg_RotateZ, WorldTransform);
	renderer._objectChanged = true;
}

void TW_CALL TranslateWorld(void* data) {
	myScene._object.Translate(Wg_TranslateX, Wg_TranslateY, Wg_TranslateZ, WorldTransform);
	renderer._objectChanged = true;
}

void TW_CALL ViewMatrixUpdate(void* data) {
	myScene._camera.UpdateViewer();
	renderer._objectChanged = true;
}

void TW_CALL CameraLookAt(void* data) {
	//Calculate current center
	glm::vec4 currentCenter = { myScene._object._box._center.x, myScene._object._box._center.y, myScene._object._box._center.z, 1 };
	currentCenter = myScene._object._worldTranslationMatrix * myScene._object._objectTranslationMatrix * currentCenter;
	//update Target
	myScene._camera._target = glm::vec3(currentCenter); //IS IT THE CENTER OF THE OBJECT? (did I take transformations on BBOX into account?)
	//Update Camera
	myScene._camera.UpdateViewer();
	renderer._objectChanged = true;
}

void TW_CALL ProjectionMatrixUpdateFOV(void* data) {
	myScene._camera.UpdatePerspective(myScene._camera._near, myScene._camera._far, myScene._camera._fov);
	renderer._objectChanged = true;
}

void TW_CALL ProjectionMatrixUpdateRightTop(void* data) {
	myScene._camera.UpdatePerspective(myScene._camera._near, myScene._camera._far, myScene._camera._right, myScene._camera._top);
	renderer._objectChanged = true;
}

void TW_CALL SetRenderBoxState(void* data) {
	renderer._enablePrintBox = g_renderBox;
	renderer._objectChanged = true;
}

void TW_CALL ToggleRenderFaceNormals(void* data) {
	renderer._enablePrintFaceNormals = g_renderFaceNormals;
	renderer._objectChanged = true;
}

void TW_CALL ToggleRenderPointNormals(void* data) {
	renderer._enablePrintPointNormals = g_renderPointNormals;
	renderer._objectChanged = true;
}

void TW_CALL SetRenderObjectAxisState(void* data) {
	renderer._objectAxis = g_renderObjectAxis;
	renderer._objectChanged = true;
}
void TW_CALL SetRenderWorldAxisState(void* data) {
	renderer._worldAxis = g_renderWorldAxis;
	renderer._objectChanged = true;
}

void TW_CALL SetObjectColor(void* data) {
	renderer._color = g_ObjectColor;
	renderer._objectChanged = true;
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

	//TODO - Try adding rendering of the object here

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

/*
void drawObject() { //IDEA - call to renderer like from drawScene and allow for multiple calls
	std::vector<Pixel> pixels;
	pixels.reserve(100000);

}
*/

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

	//draw the scene
	if (myScene._hasObject)
	{
		//drawObject();
		renderer.RenderScene(myScene);
	}
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

	//Update renderer variables
	renderer._height = height;
	renderer._width = width;
	//Update the aspect ratio for camera projection matrix
	myScene._camera.UpdateAspectRatio(width / height);
	//Update the viewport matrix
	renderer.CalculateViewPortMatrix(width, height);
	//Update the z-buffer
	renderer._zBuffer.resize(width * height);
	//Update the object
	renderer._objectChanged = true;
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
