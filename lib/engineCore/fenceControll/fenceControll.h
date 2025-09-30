#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <dxgi1_6.h>

struct FenceControll
{
	//[ Fence ]
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent = nullptr;

	void SetFence(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_);
	void Initialize(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_);

};

