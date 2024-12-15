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
    glm::mat4 _ModelViewMatrix;

    //void update(); // Update objects in the scene
    //void rasterize(std::vector<Pixel>& pixels); // Convert scene to pixel data
    Scene();
    ~Scene();
    void addObject(const Object& obj);
    void addCamera(const Camera& cam);
    void DrawScene();
    std::vector<Object> getObjects();
};

