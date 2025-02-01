#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : _viewportMatrix(1.0f), _pixels()
{
	_pixels.reserve(1000000);
	_shader = Shader();
	_zBuffer = {};
}

Renderer::~Renderer()
{
}

void Renderer::RenderScene(Scene& scene) {
	/*
	   Function: Rerenders scene
	   Purpose: Rerender if there was no change in order to reduce redundant computing.
	*/
	if (!_objectChanged) {
		drawPixels(_pixels);
		std::cout << "Object Rerendered" << std::endl;
		return;
	}

	/*
	   Function: Render Scene
	   Purpose: Renders scene according to latest updates
		- Clear Pixels from previous render
		- Get Object from Scene
		- Generate Scene Matrix for all the transformations
		- Transform Object Vertices to Viewport
		- Hidden Surface Removal
		- Clipping
		- Choose Rendering Type and Render Object
		- Additional Rendering Options
	*/
	//Clear Pixels
	_pixels.clear();
	std::fill(_zBuffer.begin(), _zBuffer.end(), std::numeric_limits<float>::max());
	std::cout << "Pixels Cleared" << std::endl;
	//Get Object
	Object objFinal = scene.getObject();
	//Generate Scene Matrix
	glm::mat4 FinalMatrix = _viewportMatrix * scene.GenerateScene();


	//Adjust to Viewport
	for (glm::vec4& point : objFinal._meshModel._points) { // CHANGE TO ITERATE OVER FACES INSTEAD OF POINTS
		//Viewport Transform
		point = FinalMatrix * point;
		//Clipping

		//Perspective Divide
		point = (point.w == 0) ? point : point / point.w;
		//Update Z-Buffer at index x + y * width
		int index = std::round(static_cast<int>(point.x)) + std::round(static_cast<int>(point.y)) * _width;
		if (index > _zBuffer.size())
			continue;
		_zBuffer[index] = std::min(_zBuffer[index], point.z);
	}
	/*
	if (_wireframe) {
		//Adjust to Viewport
		for (glm::vec4& point : objFinal._meshModel._points) { // CHANGE TO ITERATE OVER FACES INSTEAD OF POINTS
			//Viewport Transform
			point = FinalMatrix * point;
			//Clipping

			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
			//Update Z-Buffer at index x + y * width
			int index = std::round(static_cast<int>(point.x)) + std::round(static_cast<int>(point.y)) * _width;
			if (index > _zBuffer.size())
				continue;
			_zBuffer[index] = std::min(_zBuffer[index], point.z);
		}
	}
	else {
		for (auto& faceNormal : objFinal._meshModel._faceNormals) {
			glm::vec4& point1 = objFinal._meshModel._points[faceNormal.first[0]];
			glm::vec4& point2 = objFinal._meshModel._points[faceNormal.first[1]];
			glm::vec4& point3 = objFinal._meshModel._points[faceNormal.first[2]];
			//Viewport Transform
			point1 = FinalMatrix * point1;
			point2 = FinalMatrix * point2;
			point3 = FinalMatrix * point3;

			bool outside = false;
			if (point1.x < -point1.w && point2.x < -point2.w && point3.x < -point3.w) {
				outside = true;
			}
			if (point1.x > point1.w && point2.x > point2.w && point3.x > point3.w) {
				outside = true;
			}
			if (point1.y < -point1.w && point2.y < -point2.w && point3.y < -point3.w) {
				outside = true;
			}
			if (point1.y > point1.w && point2.y > point2.w && point3.y > point3.w) {
				outside = true;
			}
			if (point1.z < -point1.w && point2.z < -point2.w && point3.z < -point3.w) {
				outside = true;
			}
			if (point1.z > point1.w && point2.z > point2.w && point3.z > point3.w) {
				outside = true;
			}

			if (!outside) {
				//Transform Point

				//Clipping
				//Perspective Divide
				point1 = (point1.w == 0) ? point1 : point1 / point1.w;
				point2 = (point2.w == 0) ? point2 : point2 / point2.w;
				point3 = (point3.w == 0) ? point3 : point3 / point3.w;
				//Update Z-Buffer at index x + y * width
				int index1 = std::round(static_cast<int>(point1.x)) + std::round(static_cast<int>(point1.y)) * _width;
				int index2 = std::round(static_cast<int>(point2.x)) + std::round(static_cast<int>(point2.y)) * _width;
				int index3 = std::round(static_cast<int>(point3.x)) + std::round(static_cast<int>(point3.y)) * _width;
				if (index1 > _zBuffer.size() || index2 > _zBuffer.size() || index3 > _zBuffer.size())
					continue;
				_zBuffer[index1] = std::min(_zBuffer[index1], point1.z);
				_zBuffer[index2] = std::min(_zBuffer[index2], point1.z);
				_zBuffer[index3] = std::min(_zBuffer[index3], point1.z);

			}
		}
	}
	*/

	//Transform Normals 
	for (pair<std::array<int, 3>, glm::vec4>& point : objFinal._meshModel._faceNormals) {
		//Caclaute transformed normal (after possible rotations)
		point.second = objFinal._worldRotationMatrix * objFinal._objectRotationMatrix * point.second;
	}
	for (pair<const int, glm::vec4>& point : objFinal._meshModel._pointNormals) {
		//Caclaute transformed normal (after possible rotations)
		point.second = objFinal._worldRotationMatrix * objFinal._objectRotationMatrix * point.second;
	}

	//2nd object to save point at world coordinates
	Object obj_world_coordinates = scene.getObject();
	glm::mat4 WorldMatrix = scene.GenerateWorld();
	for (glm::vec4& point : obj_world_coordinates._meshModel._points) {
		//Viewport Transform
		point = WorldMatrix * point;
	}

	//Choose Rendering Type (Wireframe/Shading type)
	switch (_renderType) {
	case RenderType::Wireframe:
		RenderWireframe(objFinal);
		break;

	case RenderType::FlatShading:
		//Hidden Surface Removal

		_shader.RenderFlatShading(scene, _pixels, objFinal, obj_world_coordinates, _zBuffer, _width, _height);
		break;

	case RenderType::GouraudShading:
		//Hidden Surface Removal

		_shader.RenderGouraudShading(scene, _pixels, objFinal, obj_world_coordinates, _zBuffer, _width, _height);
		break;

	case RenderType::PhongShading:
		//Hidden Surface Removal

		_shader.RenderPhongShading(scene, _pixels, objFinal, obj_world_coordinates, _zBuffer, _width, _height);
		break;
	}

	//Rendering Options
	if (_worldAxis) {
		//Calculate World Axis transform matrix
		glm::mat4 worldAxisTransform = _viewportMatrix * scene._camera._projectionMatrix * scene._camera._viewMatrix;
		//Transform World Axis
		for (glm::vec4& point : objFinal._WorldAxisPoints) {
			//Viewport Transform
			point = worldAxisTransform * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render World Axis
		RenderWorldAxis(objFinal);
	}
	if (_objectAxis) {
		glm::mat4 objectAxisTransform = _viewportMatrix * scene._camera._projectionMatrix * scene._camera._viewMatrix * scene._object._worldTranslationMatrix * scene._object._worldRotationMatrix * scene._object._worldScaleMatrix;
		for (glm::vec4& point : objFinal._ObjectAxisPoints) {
			//Viewport Transform
			point = objectAxisTransform * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render Object Axis
		RenderObjectAxis(objFinal);
	}
	if (_enablePrintBox) {
		//Transform BBox
		for (glm::vec4& point : objFinal._box._boxPoints) {
			//Viewport Transform			
			point = FinalMatrix * point;
			//Perspective Divide
			point = (point.w == 0) ? point : point / point.w;
		}
		//Render BBox
		RenderBox(objFinal);
	}
	if (_enablePrintFaceNormals) {
		RenderFaceNormals(objFinal);
	}
	if (_enablePrintPointNormals) {
		RenderPointNormals(objFinal);
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
		//Calculate Normal Endpoint
		float scale = 20.0f;
		glm::vec4 normalEndpoint = centroid + (scale * point.second);

		//Draw line between points
		drawLine(static_cast<int>(std::round(centroid.x)), static_cast<int>(std::round(centroid.y)), static_cast<int>(std::round(normalEndpoint.x)), static_cast<int>(std::round(normalEndpoint.y)), _pixels, 0xfffffff);
	}
}

void Renderer::RenderPointNormals(const Object& obj) {
	for (const pair<int, glm::vec4>& point : obj._meshModel._pointNormals) {
		//Calculate Normal Endpoint
		float scale = 20.0f;
		glm::vec4 normalEndpoint = obj._meshModel._points[point.first] + (scale * point.second);
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
