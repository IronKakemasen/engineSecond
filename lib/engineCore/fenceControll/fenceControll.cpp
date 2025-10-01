#include "fenceControll.h"
#include <assert.h>
#include "../dxCommon/dxCommon.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void FenceControll::WaitFenceEvent(ID3D12CommandQueue* commandQueue_)
{
	//Fenceの値を更新
	fenceValue++;
	//GPUがここまでたどり着いたとき、
	//fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence.Get(), fenceValue);
	//fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はfence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue)
	{
		//指定したSignalにたどり着いていないので、たどり着くまで待つ
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}


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
