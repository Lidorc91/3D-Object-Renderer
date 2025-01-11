#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : _viewportMatrix(1.0f), _pixels()
{
	_pixels.reserve(1000000);
	_shader = Shader();
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
	//Generate Scene Matrix
	glm::mat4 FinalMatrix = _viewportMatrix * scene.GenerateScene();
	//glm::mat4 xSceneMatrix = scene.GenerateScene();
	//Adjust to Viewport

		//Transform Object Vertices
	for (glm::vec4& point : obj._meshModel._points) {
		//Viewport Transform
		point = FinalMatrix * point;
		//Perspective Divide
		point = (point.w == 0) ? point : point / point.w;
	}

	//Choose Rendering Type (Wireframe/Shading type)
	switch (_renderType) {
	case RenderType::Wireframe:
		RenderWireframe(obj);
		break;
	
	case RenderType::FlatShading:
		_shader.RenderFlatShading(obj);
		break;
	
	case RenderType::GouraudShading:
		_shader.RenderGouraudShading(obj);
		break;
	
	case RenderType::PhongShading:
		_shader.RenderPhongShading(obj);
		break;
	}

	//Rendering Options
	if (_worldAxis) {
		//Calculate World Axis transform matrix
		glm::mat4 worldAxisTransform = _viewportMatrix * scene._camera._projectionMatrix * scene._camera._viewMatrix;
		//Transform World Axis
		for (glm::vec4& point : obj._WorldAxisPoints) {
			//Viewport Transform
			point = worldAxisTransform * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render World Axis
		RenderWorldAxis(obj);
	}
	if (_objectAxis) {
		glm::mat4 objectAxisTransform = _viewportMatrix * scene._camera._projectionMatrix * scene._camera._viewMatrix * scene._object._worldTranslationMatrix * scene._object._worldRotationMatrix * scene._object._worldScaleMatrix;
		for (glm::vec4& point : obj._ObjectAxisPoints) {
			//Viewport Transform
			point = objectAxisTransform * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render Object Axis
		RenderObjectAxis(obj);
	}
	if (_enablePrintBox) {
		//Transform BBox
		for (glm::vec4& point : obj._box._boxPoints) {
			//Viewport Transform			
			point = FinalMatrix * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render BBox
		RenderBox(obj);
	}
	if (_enablePrintFaceNormals) {
		RenderFaceNormals(obj);
	}
	if (_enablePrintPointNormals) {
		RenderPointNormals(obj);
	}

	//Draw pixels
	drawPixels(_pixels);
	std::cout << "Object Rendered" << std::endl;
	_objectChanged = false;
}

void Renderer::RenderWireframe(const Object& obj) {
	//Push Object points to pixels
	for (const pair<int, int> edge : obj._meshModel._edges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._meshModel._points[edge.first].x));
		int y1 = static_cast<int>(std::round(obj._meshModel._points[edge.first].y));
		int x2 = static_cast<int>(std::round(obj._meshModel._points[edge.second].x));
		int y2 = static_cast<int>(std::round(obj._meshModel._points[edge.second].y));
		//Draw line between points
		drawLine(x1, y1, x2, y2, _pixels, _color);
	}
}

void Renderer::RenderBox(const Object& obj) {
	//Push BBox points to pixels
	for (const pair<int, int> boxEdge : obj._box._boxEdges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._box._boxPoints[boxEdge.first].x));
		int y1 = static_cast<int>(std::round(obj._box._boxPoints[boxEdge.first].y));
		int x2 = static_cast<int>(std::round(obj._box._boxPoints[boxEdge.second].x));
		int y2 = static_cast<int>(std::round(obj._box._boxPoints[boxEdge.second].y));

		//Draw line between points
		drawLine(x1, y1, x2, y2, _pixels, _color);
	}
}

void Renderer::RenderObjectAxis(const Object& obj) {
	//Push Axis points to pixels
	for (const pair<int, int> axisEdge : obj._ObjectAxisEdges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._ObjectAxisPoints[axisEdge.first].x));
		int y1 = static_cast<int>(std::round(obj._ObjectAxisPoints[axisEdge.first].y));
		int x2 = static_cast<int>(std::round(obj._ObjectAxisPoints[axisEdge.second].x));
		int y2 = static_cast<int>(std::round(obj._ObjectAxisPoints[axisEdge.second].y));
		//Draw line between points
		drawLine(x1, y1, x2, y2, _pixels, 0xff64ff64);
	}
}

void Renderer::RenderWorldAxis(const Object& obj) {
	//Push Axis points to pixels
	for (const pair<int, int>& axisEdge : obj._WorldAxisEdges) {
		//Create Integers for points
		int x1 = static_cast<int>(std::round(obj._WorldAxisPoints[axisEdge.first].x));
		int y1 = static_cast<int>(std::round(obj._WorldAxisPoints[axisEdge.first].y));
		int x2 = static_cast<int>(std::round(obj._WorldAxisPoints[axisEdge.second].x));
		int y2 = static_cast<int>(std::round(obj._WorldAxisPoints[axisEdge.second].y));
		//Draw line between points
		drawLine(x1, y1, x2, y2, _pixels, 0xff64ffff);
	}
}

void Renderer::RenderFaceNormals(const Object& obj) {
	for (const pair<std::array<int, 3>, glm::vec4>& point : obj._meshModel._faceNormals) {
		glm::vec4 centroid = glm::vec4(0, 0, 0, 0);
		//Calculate Centroid
		centroid += obj._meshModel._points[point.first[0]];
		centroid += obj._meshModel._points[point.first[1]];
		centroid += obj._meshModel._points[point.first[2]];
		centroid /= 3.0f;
		//Caclaute transformed normal (after possible rotations)
		glm::vec4 transformedNormal = obj._worldRotationMatrix * obj._objectRotationMatrix * point.second;
		//Calculate Normal Endpoint
		float scale = 20.0f;
		glm::vec4 normalEndpoint = centroid + (scale * transformedNormal);		

		//Draw line between points
		drawLine(static_cast<int>(std::round(centroid.x)), static_cast<int>(std::round(centroid.y)), static_cast<int>(std::round(normalEndpoint.x)), static_cast<int>(std::round(normalEndpoint.y)), _pixels, 0xfffffff);
	}
}

void Renderer::RenderPointNormals(const Object& obj) {
	for (const pair<int, glm::vec4>& point : obj._meshModel._pointNormals) {
		//Caclaute transformed normal (after possible rotations)
		glm::vec4 transformedNormal = obj._worldRotationMatrix * obj._objectRotationMatrix * point.second;
		//Calculate Normal Endpoint
		float scale = 20.0f;
		glm::vec4 normalEndpoint = obj._meshModel._points[point.first] + (scale * transformedNormal);
		//Draw line between points
		drawLine(static_cast<int>(std::round(obj._meshModel._points[point.first].x)), static_cast<int>(std::round(obj._meshModel._points[point.first].y)), static_cast<int>(std::round(normalEndpoint.x)), static_cast<int>(std::round(normalEndpoint.y)), _pixels, 0xfffffff);
	}

}

void Renderer::drawLine(int x1, int y1, int x2, int y2, std::vector<Pixel>& pixels, unsigned int color) {

	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
	int err = dx - dy, e2;
	Pixel pixel;
	while (true) {
		pixel.x = x1;
		pixel.y = y1;
		pixel.color = color;
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
	float factor = (width > height) ? width : height;
	_viewportMatrix[0][0] = factor;    // Scale X
	_viewportMatrix[1][1] = factor;   // Scale Y
	_viewportMatrix[3][0] = halfWidth;  // Translate X
	_viewportMatrix[3][1] = halfHeight; // Translate Y
}