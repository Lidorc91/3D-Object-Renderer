#pragma once

#include <vector>
#include <freeglut/include/GL/freeglut.h>
#include "Scene.h"
#include "Shader.h"

enum class RenderType
{
	Wireframe,
	FlatShading,
	GouraudShading,
	PhongShading
};



class Renderer
{
public:
	unsigned int _color = 0xfffffffff;
	glm::mat4 _viewportMatrix;

	//Render options
	bool _enablePrintBox = true;
	bool _enablePrintFaceNormals = false;
	bool _enablePrintPointNormals = false;
	bool _objectChanged = true; // Check if object has been changed
	bool _objectAxis = true;
	bool _worldAxis = true;
	RenderType _renderType = RenderType::Wireframe;

	Renderer();
	~Renderer();
	void RenderWireframe(const Object& obj);

	void RenderWorldAxis(const Object& obj);
	void RenderObjectAxis(const Object& obj);
	void RenderScene(Scene& scene);
	void RenderBox(const Object& obj);
	void RenderFaceNormals(const Object& obj);
	void RenderPointNormals(const Object& obj);
	void drawLine(int x1, int y1, int x2, int y2, std::vector<Pixel>& pixels, unsigned int color);
	void drawPixels(const std::vector<Pixel>& pixels);

	void CalculateViewPortMatrix(int width, int height);

private:
	std::vector<Pixel> _pixels;
	Shader _shader;
};
