#include "Renderer.h"
#include <iostream>

float Renderer::_screenWidth = 0;
float Renderer::_screenHeight = 0;

Renderer::Renderer() : _viewportMatrix(1.0f), _pixels()
{
	_pixels.reserve(10000000);
}


Renderer::~Renderer()
{
}

void Renderer::RenderScene(Scene& scene) {
	/*
	//Clear screen
	glClearColor(0, 0, 0, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/
	//Calculate Viewport Matrix
	CalculateViewPortMatrix();
	//Adjust to Viewport
	for (Object& obj : scene.getObjects()) {
		//Transform each point
		for (glm::vec4& point : obj._meshModel._points) {
			//Viewport Transform
			point = _viewportMatrix * point;
		}
	}
	//Render scene	
	for (Object& obj : scene.getObjects()) {
		RenderObject(obj);
	}
	/*
	//Draw tweak bars
	TwDraw();
	//Swap back and front frame buffers
	glutSwapBuffers();
	*/
}
void Renderer::RenderObject(const Object& obj) {
	for(const pair<int, int> edge : obj._meshModel._edges) {
		//Draw line between points
		drawLine(obj._meshModel._points[edge.first].x, obj._meshModel._points[edge.first].y,
			obj._meshModel._points[edge.second].x, obj._meshModel._points[edge.second].y, _pixels);
	}
	drawPixels(_pixels);
	std::cout << "Object Rendered" << std::endl;
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

void Renderer::CalculateViewPortMatrix() {
	float halfWidth = _screenWidth / 2.0f;
	float halfHeight = _screenHeight / 2.0f;

	_viewportMatrix[0][0] = halfWidth;  // Scale X
	_viewportMatrix[1][1] = halfHeight; // Scale Y
	_viewportMatrix[3][0] = halfWidth;  // Translate X
	_viewportMatrix[3][1] = halfHeight; // Translate Y
}