#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


struct SwapChainControll
{
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	D3D12_RENDER_TARGET_VIEW_DESC GetRenderTargetView(ID3D12Device* device_, ID3D12DescriptorHeap* dH_rtv_);
	//swapChainの生成
	DXGI_SWAP_CHAIN_DESC1 CreateSwapChain(HWND* hwnd_,ID3D12CommandQueue* commandQueue_, 
		IDXGIFactory7* dxgiFactory_);
	//SwapChainからリソースを引っ張る
	void PullSwapChainResource();

	DXGI_SWAP_CHAIN_DESC1 Initialize(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
		IDXGIFactory7* dxgiFactory_);
};

