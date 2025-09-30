#pragma once

#include <wrl.h>
#include <d3d12.h>

struct CommandControll
{
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	//コマンドアローケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	//コマンドリスト		
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	//コマンドキューの生成
	void MakeCommandQueue(ID3D12Device* device_);
	//コマンドアローケータの生成
	void MakeCommandAllocator(ID3D12Device* device_);
	//コマンドリストを生成する
	void MakeCommandList(ID3D12Device* device_);

	void Initialize(ID3D12Device* device_);

};

