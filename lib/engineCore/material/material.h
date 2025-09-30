#pragma once
#include "../../utilities/vectorAndMatrix/vectorAndMatrix.h"


struct Material
{
	Vector4 color;
	int enableLighting;
	float pad_enableLighting[3];
	Matrix4 uvTransform;
	float shininess = 0.5f;
	float pad_shininess[3];
};

