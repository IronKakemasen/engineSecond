#include "gpuResources.h"
#include <assert.h>

#pragma comment(lib,"d3d12.lib")

Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device_, UINT sizeInByte_)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> ret_resource;
	//頂点リソースのヒープ設定
	//upLoadHEapを使う
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//リソースのサイズ。
	resourceDesc.Width = sizeInByte_;
	//バッファの場合はこれらを1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	//決まり2
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	HRESULT hr = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&ret_resource));
	assert(SUCCEEDED(hr));

	return ret_resource;
}
