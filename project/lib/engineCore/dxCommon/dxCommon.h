#pragma once

#define DIRECTINPUT_VERSION 0x0800

//#include <DirectXMath.h>

#include "../windowSetup/windowSetUp.h"
#include "../deviceSetUp/deviceSetUp.h"
#include "../logAndException/outputDebugLog.h"
#include "../logAndException/detectException.h"
#include "../descriptorHeaps/descriptorHeaps.h"
#include "../fenceControll/fenceControll.h"
#include "../commandControll/commandControll.h"
#include "../swapChainControll/swapChainControll.h"
#include "../dxCompile/dxCompile.h"
#include "../pipelineSet/pipelineSet.h"
#include "../allShaderData/vertexShaderData.h"
#include "../allShaderData/pixelShaderData.h"
#include "../vpShaderTable/vpShaderTable.h"
#include "../depthStencilSetUp/depthStencilSetUp.h"
#include "../gpuResources/lightResources/lightResources.h"


#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx12.h"
#include "../../../external/imgui/imgui_impl_win32.h"


class DxCommon
{

public:

	static inline bool debugLayerSwitch = false;
	static inline std::ofstream debugLog;

	float windowColor[4] = { 0.1f,0.1f,0.1f,1.0f };

	DxCommon();
	void Finalize();
	void BeginFrame();
	void EndFrame();

private:

	//[ WindowSetUp ]
	WindowSetUp windowSetUp;

	//[ DeviceSetUp ]
	DeviceSetUp deviceSetUp;

	//[ CommandControll ]
	CommandControll commandControll;

	//[ FenceControll ]
	FenceControll fenceControll;

	//[ SwapChain ]
	SwapChainControll swapChainControll;

	//[ DescriptorHeaps ]
	DescriptorHeapsSet descriptorHeapSet;

	//[ DxCompile ]
	DxCompile dxCompile;

	//[ ShaderData ]
	PixelShaderData pixShaderData;
	VertexShaderData verShaderData;

	//[ VpShaderTable ]
	VPShaderTable vpShaderTable;

	// [ PipelineSet ]
	std::unique_ptr<PipelineSet> pipelineSetTriangles[kCountOfBlendMode][(int)CullMode::kCountOfCullMode][ShaderSet::Type::kCount];
	std::unique_ptr<PipelineSet> pipelineSetLines[kCountOfBlendMode][(int)CullMode::kCountOfCullMode][ShaderSet::Type::kCount];

	//[ DepthStencilSetUp ]
	DepthStencilSetUp depthStencilSetUp;

	//[ LightResources ]
	LightResources lightResources;


	void CompileAllShaders();

	void SetShaderTable();

	D3D12_DESCRIPTOR_RANGE GetShaderViewDescriptorRange();

	D3D12_RASTERIZER_DESC GetRasterizerDesc(CullMode cullMode_);

	std::unique_ptr<PipelineSet> CreateGraphicsPipelineSet(ID3D12Device* device_,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_, 
		BlendMode blendMode_,
		CullMode cullMode_, 
		ShaderSet::Type shaderSetType_);

	void CreateAllGraphicsPipelineSets(ID3D12Device* device_);


};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);


