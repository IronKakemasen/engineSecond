#pragma once

#include <wrl.h>
#include <d3d12.h>


struct ResourceAndHandleForRender
{
	static inline D3D12_CPU_DESCRIPTOR_HANDLE cur_handlePtr;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;

	void SetDXMatrix(float width_,float height_,float left_ = 0.0f, float top_ = 0.0f);
};

