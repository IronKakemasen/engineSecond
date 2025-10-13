#pragma once

#include "../gpuResources/resourceAndHandleForRender/resourceAndHandleForRender.h"
#include <dxgi1_6.h>


struct SwapChainControll
{
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ResourceAndHandleForRender swapChain_resourcesAndHandles[2];


	D3D12_RENDER_TARGET_VIEW_DESC GetRenderTargetView(ID3D12Device* device_, ID3D12DescriptorHeap* dH_rtv_);

	DXGI_SWAP_CHAIN_DESC1 Initialize(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
		IDXGIFactory7* dxgiFactory_);

private:

	//swapChainの生成
	DXGI_SWAP_CHAIN_DESC1 CreateSwapChain(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
		IDXGIFactory7* dxgiFactory_);

	//SwapChainからリソースを引っ張る
	void PullSwapChainResource();

};


