#pragma once

#include <vector>
#include "Object.h"
#include "Camera.h"

enum class LightType
{
	Point,
	Directional,
	Ambient
};

struct LightSource
{
    LightType _type = LightType::Directional; // Default to Directional
    glm::vec3 _position = { 0.0f, 0.0f, 0.0f }; // For Point lights
    glm::vec3 _direction = { 0.0f, 0.0f, -1.0f }; // For Directional lights
    glm::vec3 _color = { 1.0f, 1.0f, 1.0f }; // Intensity and color of the light

	void ChangeType(LightType type) {
		_type = type;
	}

	void ChangeColor(glm::vec3 color) {
		_color = color;
	}

	void ChangePosition(glm::vec3 position) {
		if (_type == LightType::Point)
			_position = position;
	}

	void ChangeDirection(glm::vec3 direction) {
		if (_type == LightType::Directional)
		    _direction = direction;
	}
};

class Scene
{
public:
    Object _object;
    Camera _camera;
	bool _hasObject;
	LightSource _lightSource1;
	LightSource _lightSourceOptional;
	LightSource _ambientLight;

    //void update(); // Update objects in the scene
    //void rasterize(std::vector<Pixel>& pixels); // Convert scene to pixel data
    Scene();
    void setObject(Wavefront_obj& obj);
    glm::mat4 GenerateScene();
    //std::vector<glm::mat4> Transformations; ADD WHEN IMPLEMENTING MULTIPLE OBJECTS
    Object getObject();

private:
    //glm::mat4 _viewportMatrix;

    //void CalculateViewPortMatrix();
};

