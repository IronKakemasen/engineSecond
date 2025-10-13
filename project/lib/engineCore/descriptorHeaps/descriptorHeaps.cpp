#include "descriptorHeaps.h"
#include <assert.h>
#pragma comment(lib,"d3d12.lib")

void DescriptorHeapsSet::Initialize(ID3D12Device* device_)
{
	//RTV用のヒープでディスクリプタの数は2。RTVはShader内で触るものではないので,ShaderVisibleはfalse
	dH_rtv = MakeDescriptorHeap(device_,D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxRtvDecriptors, false);
	//SRV用のヒープ。SRVはShader内で触るもなので,ShaderVisibleはtrue
	dH_srv = MakeDescriptorHeap(device_,D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxTexture, true);
	//DSV用のディスクリプタヒープ(Shader内で見るものではない)
	dH_dsv = MakeDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapsSet::MakeDescriptorHeap(ID3D12Device* device_, D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ret_descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorheapDesc{};
	descriptorheapDesc.Type = heapType_;
	descriptorheapDesc.NumDescriptors = numDescriptors_;
	descriptorheapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device_->CreateDescriptorHeap(&descriptorheapDesc,
		IID_PPV_ARGS(ret_descriptorHeap.GetAddressOf()));

	assert(SUCCEEDED(hr));

	return ret_descriptorHeap;
}


D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapsSet::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
	uint32_t descriptorSize_, uint32_t index_)
{
	D3D12_CPU_DESCRIPTOR_HANDLE ret_handleCPU = descrptorHeap_->GetCPUDescriptorHandleForHeapStart();
	ret_handleCPU.ptr += descriptorSize_ * index_;

	return ret_handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapsSet::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
	uint32_t descriptorSize_, uint32_t index_)
{
	D3D12_GPU_DESCRIPTOR_HANDLE ret_handleGPU = descrptorHeap_->GetGPUDescriptorHandleForHeapStart();
	ret_handleGPU.ptr += descriptorSize_ * index_;

	return ret_handleGPU;
}
