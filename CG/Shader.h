#pragma once
#include "Object.h"
#include "Scene.h"

struct Pixel
{
	int  x, y; //pixel in screen coordinates
	unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

class Shader
{
public:
	Shader();

	glm::vec3 CalculateIllumination(const Object& obj, const LightSource& light, const glm::vec4& normal, glm::vec3 fragmentPosition, const Camera& camera);
	std::array<pair<glm::vec3, glm::vec4>, 3> _colorPoint;
	std::vector<glm::vec4> _facePoints;
	void RenderFlatShading(const Scene& scene, std::vector<Pixel>& pixels, Object& tempObj, Object& worldObj, std::vector<float>& _zBuffer, int screenWidth, int screenHeight);
	void RenderGouraudShading(const Scene& scene, std::vector<Pixel>& pixels, Object& finalObj, Object& worldObj, std::vector<float>& zBuffer, int screenWidth, int screenHeight);
	void RenderPhongShading(const Scene& scene, std::vector<Pixel>& pixels, Object& finalObj, Object& worldObj, std::vector<float>& zBuffer, int screenWidth, int screenHeight);
	bool CheckBarycentricCoordinates(int xIndex, int yIndex, float v0x, float v0y, float v1x, float v1y, float v2x, float v2y, float denom);
};

