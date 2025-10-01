#include "swapChainControll.h"
#include <assert.h>
#include "../dxCommon/dxCommon.h"
#include "../../commonVariables.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


D3D12_RENDER_TARGET_VIEW_DESC SwapChainControll::GetRenderTargetView(ID3D12Device* device_, ID3D12DescriptorHeap* dH_rtv_)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//出力結果をSRGBに変換する
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//2Dテクスチャとして書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = dH_rtv_->GetCPUDescriptorHandleForHeapStart();
	//1つめを作る。１つ目は最初に作る。作る場所をこちらで指定する必要がある
	swapChain_resourcesAndHandles[0].rtvHandle = rtvStartHandle;
	device_->CreateRenderTargetView(swapChain_resourcesAndHandles[0].resource.Get(), 
		&rtvDesc, swapChain_resourcesAndHandles[0].rtvHandle);
	//2つめのディスクリプタハンドルを得る
	swapChain_resourcesAndHandles[1].rtvHandle.ptr = 
		swapChain_resourcesAndHandles[0].rtvHandle.ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つめを作る
	device_->CreateRenderTargetView(
		swapChain_resourcesAndHandles[1].resource.Get(), &rtvDesc, swapChain_resourcesAndHandles[1].rtvHandle);

	ResourceAndHandleForRender::cur_handlePtr.ptr = swapChain_resourcesAndHandles[1].rtvHandle.ptr;

	return rtvDesc;
}


//swapChainの生成
DXGI_SWAP_CHAIN_DESC1 SwapChainControll::CreateSwapChain(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
	IDXGIFactory7* dxgiFactory_)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//画面の縦横。クライアント領域と同じにしておく
	swapChainDesc.Width = (UINT)V_Common::kWindow_W;
	swapChainDesc.Height = (UINT)V_Common::kWindow_H;
	//色の形成
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//マルチサンプルしない
	swapChainDesc.SampleDesc.Count = 1;
	//描画のターゲットとして利用する
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//ダブルバッファ
	swapChainDesc.BufferCount = 2;
	//モニタに移したら中身を破棄
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_, *hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete create SwapChain\n");

	return swapChainDesc;
}

//SwapChainからリソースを引っ張る
void SwapChainControll::PullSwapChainResource()
{
	Log(DxCommon::debugLog, "Complete create rtvDescriptorHeap\n");

	//SwapChainからResourceを引っ張ってくる
	HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChain_resourcesAndHandles[0].resource));
	//生成失敗
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChain_resourcesAndHandles[1].resource));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete pull Resource\n");

}

DXGI_SWAP_CHAIN_DESC1 SwapChainControll::Initialize(HWND* hwnd_, ID3D12CommandQueue* commandQueue_,
	IDXGIFactory7* dxgiFactory_)
{
	DXGI_SWAP_CHAIN_DESC1 desc = CreateSwapChain(hwnd_, commandQueue_, dxgiFactory_);
	PullSwapChainResource();
	swapChain_resourcesAndHandles->SetDXMatrix(V_Common::kWindow_W, V_Common::kWindow_H);

	return desc;
}
