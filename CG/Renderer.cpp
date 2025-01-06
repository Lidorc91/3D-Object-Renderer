#include "Renderer.h"
#include <iostream>



Renderer::Renderer() : _viewportMatrix(1.0f), _pixels()
{
	_pixels.reserve(1000000);
}

Renderer::~Renderer()
{
}

void Renderer::RenderScene(Scene& scene) {
	//Check if object has been rendered
	if (!_objectChanged) {
		drawPixels(_pixels);
		std::cout << "Object Rerendered" << std::endl;
		return;
	}
	//Clear Pixels
	_pixels.clear();
	std::cout << "Pixels Cleared" << std::endl;
	//Get Object
	Object obj = scene.getObject();
	//Generate Final Matrix
	glm::mat4 FinalMatrix = _viewportMatrix * scene.GenerateScene();
	//Adjust to Viewport
		//Transform Object Vertices
	for (glm::vec4& point : obj._meshModel._points) {			
		//Viewport Transform
		point = FinalMatrix * point;
		//Perspective Divide
		point = (point.w == 0) ? point : point/point.w;
	}
		//Transform BBox
	for (glm::vec4& point : obj._box._points) {
		//Viewport Transform			
		point = FinalMatrix * point;
		//Perspective Divide
		point = (point.w == 0) ? point : point / point.w;
	}
	
	//Render scene	
	RenderObject(obj);
	if (_enablePrintBox) {
		RenderBox(obj);
	}
	if (_enablePrintNormals) {
		RenderNormals(obj);
	}

	//Draw pixels
	drawPixels(_pixels);
	std::cout << "Object Rendered" << std::endl;
	_objectChanged = false;
}

void Renderer::RenderObject(const Object& obj) {
	//Push Object points to pixels
	for(const pair<int, int> edge : obj._meshModel._edges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._meshModel._points[edge.first].x));
		int y1 = static_cast<int>(std::round(obj._meshModel._points[edge.first].y));
		int x2 = static_cast<int>(std::round(obj._meshModel._points[edge.second].x));
		int y2 = static_cast<int>(std::round(obj._meshModel._points[edge.second].y));
		//Draw line between points
		drawLine(x1,y1,x2,y2, _pixels);
	}
}

void Renderer::RenderBox(const Object& obj) {
	//Push BBox points to pixels
	for (const pair<int, int> edge : obj._box._edges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._box._points[edge.first].x));
		int y1 = static_cast<int>(std::round(obj._box._points[edge.first].y));
		int x2 = static_cast<int>(std::round(obj._box._points[edge.second].x));
		int y2 = static_cast<int>(std::round(obj._box._points[edge.second].y));

		//Draw line between points
		drawLine(x1, y1, x2, y2, _pixels);
	}
}

void Renderer::RenderNormals(const Object& obj) {
	for (const pair<std::array<int, 3>, glm::vec4>& point : obj._meshModel._normals) {
		glm::vec4 centroid = glm::vec4(0, 0, 0, 0);
		//Calculate Centroid
		centroid += obj._meshModel._points[point.first[0]];
		centroid += obj._meshModel._points[point.first[1]];
		centroid += obj._meshModel._points[point.first[2]];
		centroid /= 3.0f;
		//Calculate Normal Endpoint
		float scale = 20.0f;
		glm::vec4 normalEndpoint = centroid + (scale * point.second);

		//Draw line between points
		drawLine(static_cast<int>(std::round(centroid.x)), static_cast<int>(std::round(centroid.y)), static_cast<int>(std::round(normalEndpoint.x)), static_cast<int>(std::round(normalEndpoint.y)), _pixels);
	}
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, std::vector<Pixel>& pixels) {

	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
	int err = dx - dy, e2;
	Pixel pixel;
	while (true) {
		pixel.x = x1;
		pixel.y = y1;
		pixel.color = 0xffff0000;
		pixels.push_back(pixel);
		if (x1 == x2 && y1 == y2) break;

		e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x1 += sx; }
		if (e2 < dx) { err += dx; y1 += sy; }
	}
}

//this function turns on the specified pixels on screen
void Renderer::drawPixels(const std::vector<Pixel>& pixels)
{
	int numPixels = pixels.size();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, sizeof(Pixel), &pixels[0].x);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Pixel), &pixels[0].color);

	glDrawArrays(GL_POINTS, 0, numPixels);
}

//Turn on/off world/object coordinate

//Add viewport transform (center camera)

void Renderer::CalculateViewPortMatrix(int width, int height) {
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	_viewportMatrix[0][0] = halfWidth;  // Scale X
	_viewportMatrix[1][1] = halfHeight; // Scale Y
	_viewportMatrix[3][0] = halfWidth;  // Translate X
	_viewportMatrix[3][1] = halfHeight; // Translate Y
}