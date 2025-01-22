#include "Shader.h"
#include <iostream>

Shader::Shader()
{
}

void Shader::RenderFlatShading(const Scene& scene, std::vector<Pixel>& pixels)
{
	//aliases for ease of use
	const Object& obj = scene._object;
	const Camera& camera = scene._camera;
	const std::vector<glm::vec4>& points = obj._meshModel._points;

	//for each face in object calculate illumination
	for (const auto& faceNormal : obj._meshModel._faceNormals) {
		//Aliases for ease of use
		const float& v0x = points[faceNormal.first[0]].x;
		const float& v0y = points[faceNormal.first[0]].y;
		const float& v1x = points[faceNormal.first[1]].x;
		const float& v1y = points[faceNormal.first[1]].y;
		const float& v2x = points[faceNormal.first[2]].x;
		const float& v2y = points[faceNormal.first[2]].y;
		//Calculate Centroid
		glm::vec3 centroid = (obj._meshModel._points[faceNormal.first[0]] + obj._meshModel._points[faceNormal.first[1]] + obj._meshModel._points[faceNormal.first[2]]) / 3.0f;
		//Calculate Illumination
		glm::vec3 I = CalculateIllumination(obj, scene._lightSource1, faceNormal, centroid, camera) + CalculateIllumination(obj, scene._lightSourceOptional, faceNormal, centroid, camera) + CalculateIllumination(obj, scene._ambientLight, faceNormal, centroid, camera);
		//Draw face (add to pixel vector)
			//scale color to 0-255
		unsigned int color = 0xff000000 + (static_cast<int>(I.r * 255) << 16) + (static_cast<int>(I.g * 255) << 8) + static_cast<int>(I.b * 255);
			//Create bounding box for face
		int minX = std::min({ obj._meshModel._points[faceNormal.first[0]].x, obj._meshModel._points[faceNormal.first[1]].x, obj._meshModel._points[faceNormal.first[2]].x });
		int minY = std::min({ obj._meshModel._points[faceNormal.first[0]].y, obj._meshModel._points[faceNormal.first[1]].y, obj._meshModel._points[faceNormal.first[2]].y });
		int maxX = std::max({ obj._meshModel._points[faceNormal.first[0]].x, obj._meshModel._points[faceNormal.first[1]].x, obj._meshModel._points[faceNormal.first[2]].x });
		int maxY = std::max({ obj._meshModel._points[faceNormal.first[0]].y, obj._meshModel._points[faceNormal.first[1]].y, obj._meshModel._points[faceNormal.first[2]].y });
		float denom = ((obj._meshModel._points[faceNormal.first[1]].y - obj._meshModel._points[faceNormal.first[2]].y) * (obj._meshModel._points[faceNormal.first[0]].x - obj._meshModel._points[faceNormal.first[2]].x) + (obj._meshModel._points[faceNormal.first[2]].x - obj._meshModel._points[faceNormal.first[1]].x) * (obj._meshModel._points[faceNormal.first[0]].y - obj._meshModel._points[faceNormal.first[2]].y));

			//Iterate over bounding box and check if it's a barycentric coordinate (shade if it is)
		for (int y = minY; y <= maxY; y++) {
			for (int x = minX; x <= maxX; x++) {
				if (CheckBarycentricCoordinates(x, y , v0x, v0y , v1x,v1y,v2x,v2y, denom)) //Check if point is in triangle
					pixels.push_back({ x,y,color });
			}
		}
	}
	
}


bool Shader::CheckBarycentricCoordinates(int xIndex, int yIndex ,float v0x, float v0y ,float v1x, float v1y, float v2x, float v2y, float denom) {
	float lambda1 = ((v1y - v2y) * (xIndex - v2x) + (v2x - v1x) * (yIndex - v2y)) / denom;
	float lambda2 = ((v2y - v0y) * (xIndex - v2x) + (v0x - v2x) * (yIndex - v2y)) / denom;
	float lambda3 = 1.0f - lambda1 - lambda2;
	if (lambda1 >= 0 && lambda1 <= 1 && lambda2 >= 0 && lambda2 <= 1 && lambda3 >= 0 && lambda3 <= 1)
		return true;
	return false;
}
void Shader::RenderGouraudShading(const Scene& scene, std::vector<Pixel>& pixels)
{
	std::cout << "Gouraud Shading" << std::endl;
}

void Shader::RenderPhongShading(const Scene& scene, std::vector<Pixel>& pixels)
{
	std::cout << "Phong Shading" << std::endl;
}

//Calculate illumination equation
//I = Ia + Id + Is
//Ia = Ka * Ia
//Id = Kd * Id * max(0, N.L)
//Is = Ks * Is * max(0, N.H)^n
//N = Normal
//L = Light Direction
//H = Halfway vector
//Ka, Kd, Ks = Ambient, Diffuse, Specular reflection coefficients
//Ia, Id, Is = Ambient, Diffuse, Specular light intensities
//n = Shininess coefficient


glm::vec3 calculateAmbient(const Material& material, const LightSource& light) {
	return material._ambient * light._color;
}

glm::vec3 calculateDiffuse(const Material& material, const LightSource& light, const glm::vec3& normal, const glm::vec3& fragmentPosition) {
	glm::vec3 lightDir;

	if (light._type == LightType::Point) {
		lightDir = glm::normalize((glm::vec3)light._position - fragmentPosition); // Vector from surface to light
	}
	else if (light._type == LightType::Directional) {
		lightDir = glm::normalize(-light._direction); // Invert light's direction
	}
	else {
		return glm::vec3(0.0f); // Ambient lights don't contribute to diffuse
	}

	float diffuseStrength = glm::max(glm::dot(normal, lightDir), 0.0f); // Clamp to [0, 1]
	return material._diffuse * light._color * diffuseStrength;
}

glm::vec3 calculateSpecular(const Material& material, const LightSource& light, const glm::vec3& normal, const glm::vec3& viewDir, const glm::vec3& fragmentPosition) {
	glm::vec3 lightDir;

	if (light._type == LightType::Point) {
		lightDir = glm::normalize((glm::vec3)light._position - fragmentPosition);
	}
	else if (light._type == LightType::Directional) {
		lightDir = glm::normalize(-light._direction);
	}
	else {
		return glm::vec3(0.0f); // Ambient lights don't contribute to specular
	}

	glm::vec3 reflectDir = glm::reflect(-lightDir, normal); // Reflection vector
	float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), material._shininess);
	return material._specular * light._color * spec;
}

glm::vec3 Shader::CalculateIllumination(const Object& obj, const LightSource& light,const pair<std::array<int, 3>, glm::vec4>& faceNormal,glm::vec3 fragmentPosition,const Camera& camera)
{
	if (!light._enabled)
		return glm::vec3(0.0f);

	glm::vec3 I = glm::vec3(0.0f);
	switch (light._type)
	{
	case LightType::Ambient:
		//Calculate Ambient Light
		I += calculateAmbient(obj._material, light);
		break;
	case LightType::Point:
		//Calculate Diffuse Light
		I += calculateDiffuse(obj._material, light, faceNormal.second, fragmentPosition);
		//Calculate Specular Light
		glm::vec3 viewDir = glm::normalize(camera._eye - fragmentPosition);		
		I += calculateSpecular(obj._material, light, faceNormal.second, viewDir, fragmentPosition);
		break;
	case LightType::Directional:
		//Calculate Diffuse Light
		I += calculateDiffuse(obj._material, light, faceNormal.second, fragmentPosition);
		//Calculate Specular Light
		break;

	default:
		break;
	}
	return I;
}

