#pragma once
#include "Object.h"

class Shader
{
public:
	Shader();

	void RenderFlatShading(const Object& obj);
	void RenderGouraudShading(const Object& obj);
	void RenderPhongShading(const Object& obj);
};

