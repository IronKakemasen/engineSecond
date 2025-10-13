#include "depthStencilSetUp.h"
#include <assert.h>

#pragma comment(lib,"d3d12.lib")



D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilSetUp::GetDepthStencilViewDesc()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC ret_dsvDesc{};

	//format。基本的にはリソースに合わせる
	ret_dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//2DTexture
	ret_dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	return ret_dsvDesc;
}


void DepthStencilSetUp::CreateDepthStencilTextureResource(ID3D12Device* device_,
	int32_t width_, int32_t height_)
{
	//生成するResourceの設定
	//テクスチャのサイズ
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Width = width_;
	resourceDesc.Height = height_;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行or配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//depthstencilとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//サンプリングカウント1固定
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	//VRAM上に作る
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値クリア設定
	//最大値（1.0）でクリア
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	//フォーマット。resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resourceの生成
	HRESULT hr = device_->CreateCommittedResource
	(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定：特になし
		&resourceDesc,						//resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&depthStencilTextureResource)			//作成するリソースポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

}

void DepthStencilSetUp::Initialize(ID3D12Device* device_, int32_t width_, int32_t height_)
{
	CreateDepthStencilTextureResource(device_, width_, height_);
}
