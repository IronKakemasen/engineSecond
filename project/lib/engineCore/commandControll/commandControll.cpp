#include "commandControll.h"
#include <assert.h>
#include "../dxCommon/dxCommon.h"

#pragma comment(lib,"d3d12.lib")

void CommandControll::PrepareForNextCommandList()
{
	HRESULT hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void CommandControll::Initialize(ID3D12Device* device_)
{
	//コマンドキューの生成
	MakeCommandQueue(device_);
	//コマンドアローケータの生成
	MakeCommandAllocator(device_);
	//コマンドリストを生成する
	MakeCommandList(device_);

}

void CommandControll::MakeCommandQueue(ID3D12Device* device_)
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	HRESULT hr = device_->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成がうまくいかなかった場合はエラー
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete create CommandQueue\n");
}
void CommandControll::MakeCommandAllocator(ID3D12Device* device_)
{
	HRESULT hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	//生成がうまくいかなかった場合はエラー
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete create AlloCator\n");
}
void CommandControll::MakeCommandList(ID3D12Device* device_)
{
	HRESULT hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(),
		nullptr, IID_PPV_ARGS(&commandList));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete create CommandList\n");
}
