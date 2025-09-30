#include "fenceControll.h"
#include <assert.h>
#include "../dxCommon/dxCommon.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void FenceControll::Initialize(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_)
{
	SetFence(device_, dxgiFactory_);
}

void FenceControll::SetFence(ID3D12Device* device_, IDXGIFactory7* dxgiFactory_)
{
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	hr = device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete built fence\n");

	//FenceのSignalを待つためのイベント（WINDOWへのメッセージ）を作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
	Log(DxCommon::debugLog, "Complete creat event\n");

}
