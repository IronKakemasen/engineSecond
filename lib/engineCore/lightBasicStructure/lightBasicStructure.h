#pragma once
#include "../../utilities/vectorAndMatrix/vectorAndMatrix.h"
#include <wrl.h>
#include <d3d12.h>

struct LightResources;

struct LightBasicStructure
{
	Vector4 color = {255,255,255,255};
	Vector3 direction = { 0,1,1 };
	float intensity = 0.5f;

	virtual void CreateResourceAndMapping() = 0;

};

