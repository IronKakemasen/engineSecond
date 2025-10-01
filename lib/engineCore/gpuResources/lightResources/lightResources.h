#pragma once
#include <vector>
#include "../gpuResources.h"

struct LightResources:GPUResource
{
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> directionalLightResources;

};

