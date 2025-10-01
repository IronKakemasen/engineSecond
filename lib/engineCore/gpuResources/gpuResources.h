#pragma once

#include <wrl.h>
#include <d3d12.h>

struct GPUResource
{
	ID3D12Device* device = nullptr;
};

Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device_,
	UINT sizeInByte_);
