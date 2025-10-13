#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>

struct DescriptorHeapsSet
{
	//[ DescriptorHeap ]
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dH_rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dH_srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dH_dsv = nullptr;

	//[ DescriptorSize ]
	static inline uint32_t sizeSRV;
	static inline uint32_t sizeRTV;
	static inline uint32_t sizeDSV;

	//texture最大生成数
	uint8_t const kMaxTexture = 128;
	//rtvDの最大数
	uint8_t const kMaxRtvDecriptors = 16;


	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(ID3D12Device* device_,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible);

	void Initialize(ID3D12Device* device_);

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
		uint32_t descriptorSize_, uint32_t index_);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
		uint32_t descriptorSize_, uint32_t index_);

};

