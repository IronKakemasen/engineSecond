#pragma once

#include <dxgi1_6.h>
#include <wrl.h>
#include <d3d12.h>


struct DeviceSetUp
{
	//[ Device ]
	//DXGIファクトリーの作成
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	//使用するアダプタ用の変数。
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

	void Initialize();

private:
	void SetGoodDevice();

};

