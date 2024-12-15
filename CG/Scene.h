#pragma once

#include <vector>
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
    std::vector<Object> _objects;
    std::vector<Camera> _cameras;
    static Camera* _viewer;
    

    //void update(); // Update objects in the scene
    //void rasterize(std::vector<Pixel>& pixels); // Convert scene to pixel data
    Scene(int width, int height);
    ~Scene();
    void addObject(const Object& obj);
    void addCamera(const Camera& cam);
    void DrawScene();
    std::vector<Object> getObjects();

private:
    static float _screenWidth;
    static float _screenHeight;
    glm::mat4 _viewportMatrix;

    void CalculateViewPortMatrix();
};

