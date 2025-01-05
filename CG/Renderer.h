#pragma once

#include <vector>
#include <freeglut/include/GL/freeglut.h>
#include "Scene.h"

struct Pixel
{
	int  x, y; //pixel in screen coordinates
	unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

class Renderer
{
public:
	glm::mat4 _viewportMatrix;
	bool _enablePrintBox = true;
	bool _enablePrintNormals = true;
	bool _objectChanged = true; // Check if object has been changed
	
	Renderer();
	~Renderer();

	void RenderScene(Scene& scene);
	void RenderObject(const Object& obj);
	void RenderBox(const Object& obj);
	void RenderNormals(const Object& obj);
	void drawLine(int x1, int y1, int x2, int y2, std::vector<Pixel>& pixels);
	void drawPixels(const std::vector<Pixel>& pixels);
	
	void CalculateViewPortMatrix(int width, int height);

private:
	std::vector<Pixel> _pixels;
};
