#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>

struct DepthStencilSetUp
{
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilTextureResource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;


	D3D12_DEPTH_STENCIL_VIEW_DESC GetDepthStencilViewDesc();
	void Initialize(ID3D12Device* device_, int32_t width_, int32_t height_);


private:
	void CreateDepthStencilTextureResource(ID3D12Device* device_,
		int32_t width_, int32_t height_);

};

