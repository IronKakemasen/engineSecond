#include "DxCommon.h"
#include <filesystem>
#include <assert.h>
#include "../../commonVariables.h"

//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"Dbghelp.lib")
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"winmm.lib")

DxCommon::DxCommon()
{
	//COMの初期化
	CoInitializeEx(0, COINITBASE_MULTITHREADED);

	//[ DebugLog ]
	debugLog = DebugLogInitialize();

	//[ デバッグヘルパー ]
	//誰も捕捉しなかった場合に(Unhandled)、補足する関数を登録
	SetUnhandledExceptionFilter(ExportDump);

	//windowの初期化
	windowSetUp.Initialize(L"2007_シフトトゥ_1.1");

	//デバイス探してセットする
	deviceSetUp.Initialize();
	//デバイスのアドレスを渡す
	lightResources.device = deviceSetUp.device.Get();

	//フェンスの設定
	fenceControll.Initialize(deviceSetUp.device.Get(), deviceSetUp.dxgiFactory.Get());

	//コンパイラーの初期化
	dxCompile.Initialize();

	//コマンドキュー,コマンドアローケータ,コマンドリストの生成
	commandControll.Initialize(deviceSetUp.device.Get());
	//コマンドキューのアドレスを渡す



	//swapChainの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = swapChainControll.Initialize(
		&windowSetUp.hwnd, commandControll.commandQueue.Get(), deviceSetUp.dxgiFactory.Get());

	//各種descriptorHeapの初期化
	descriptorHeapSet.Initialize(deviceSetUp.device.Get());

	//メインのdepthStencilの初期化
	depthStencilSetUp.Initialize(deviceSetUp.device.Get(), (int)V_Common::kWindow_W,
		(int)V_Common::kWindow_H);

	//すべてのシェーダーをコンパイルする（いつか個別ごとに変更するかも）
	CompileAllShaders();

	//シェーダーテーブルをセットする
	SetShaderTable();

	//すべてのGraphicsPipelineを生成（いつか個別ごとに変更するかも）
	CreateAllGraphicsPipelineSets(deviceSetUp.device.Get());

	//swapChainのRTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = 
		swapChainControll.GetRenderTargetView(deviceSetUp.device.Get(),descriptorHeapSet.dH_rtv.Get());

	//DSVHeapの先頭にDSVを作る
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv_viewDesc = depthStencilSetUp.GetDepthStencilViewDesc();
	deviceSetUp.device->CreateDepthStencilView(depthStencilSetUp.depthStencilTextureResource.Get(),
		&dsv_viewDesc, descriptorHeapSet.dH_dsv.Get()->GetCPUDescriptorHandleForHeapStart());


	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplWin32_Init(windowSetUp.hwnd);
	ImGui_ImplDX12_Init(
		deviceSetUp.device.Get(),
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		descriptorHeapSet.dH_srv.Get(),
		DescriptorHeapsSet::GetCPUDescriptorHandle(descriptorHeapSet.dH_srv.Get(), 
		DescriptorHeapsSet::sizeSRV, 0),		//SRVHeap上の０番目
		DescriptorHeapsSet::GetGPUDescriptorHandle(descriptorHeapSet.dH_srv.Get(),
		DescriptorHeapsSet::sizeSRV, 0));

}


void DxCommon::Finalize()
{
	CloseHandle(fenceControll.fenceEvent);

	//[ ImGui ]
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	CloseWindow(windowSetUp.hwnd);

	//COMの終了
	CoUninitialize();

}

void DxCommon::SetShaderTable()
{
	int const kNumSet = ShaderSet::Type::kCount;

	//Default Default
	vpShaderTable.Set(ShaderSet::Type::kDefaultAndDefault,
		verShaderData.data[VertexShaderData::Type::kDefault].second.Get(),
		pixShaderData.data[PixelShaderData::Type::kDefault].second.Get());

	//Default Kirei
	vpShaderTable.Set(ShaderSet::Type::kDefaulAndKirei,
		verShaderData.data[VertexShaderData::Type::kDefault].second.Get(),
		pixShaderData.data[PixelShaderData::Type::kKirei].second.Get());

	//Default Test
	vpShaderTable.Set(ShaderSet::Type::kDefaultAndTest,
		verShaderData.data[VertexShaderData::Type::kDefault].second.Get(),
		pixShaderData.data[PixelShaderData::Type::kTest].second.Get());


}

void DxCommon::CompileAllShaders()
{
	for (auto& [key, value] : verShaderData.data)
	{
		value.second = dxCompile.CompileShader(value.first,
			L"vs_6_0", dxCompile.dxcUtils.Get(), dxCompile.dxcCompiler.Get(), dxCompile.includeHandler.Get());
		assert(value.second != nullptr);
		Log(debugLog, "value.first Compiled\n");
	}

	for (auto& [key, value] : pixShaderData.data)
	{
		value.second = dxCompile.CompileShader(value.first,
			L"ps_6_0", dxCompile.dxcUtils.Get(), dxCompile.dxcCompiler.Get(), dxCompile.includeHandler.Get());
		assert(value.second != nullptr);
		Log(debugLog, "value.first Compiled\n");
	}

}

D3D12_DESCRIPTOR_RANGE DxCommon::GetShaderViewDescriptorRange()
{
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return descriptorRange[0];
}

D3D12_RASTERIZER_DESC DxCommon::GetRasterizerDesc(CullMode cullMode_)
{
	D3D12_RASTERIZER_DESC ret_rasterizerDesc{};

	//カルモード
	ret_rasterizerDesc.CullMode = D3D12_CULL_MODE((int)cullMode_ + 1);

	//三角形の中身を塗りつぶす
	ret_rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return ret_rasterizerDesc;
}

std::unique_ptr<PipelineSet> DxCommon::CreateGraphicsPipelineSet(ID3D12Device* device_,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_, 
	BlendMode blendMode_,
	CullMode cullMode_, 
	ShaderSet::Type shaderSetType_)
{
	//return 
	std::unique_ptr<PipelineSet> ret_pipelineSet = std::make_unique<PipelineSet>();

	//graphiscPipelineDesc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graghicsPipeLineStatedesc{};

	//vertexShaderBlob
	IDxcBlob* srcVertexShaderBlob = vpShaderTable.Get(shaderSetType_).verShaderBlob;

	//pixelShaderBlob
	IDxcBlob* srcPixelShaderBlob = vpShaderTable.Get(shaderSetType_).pixShaderBlob;

	//shaderBlobをセット(Vertex)
	graghicsPipeLineStatedesc.VS =
	{
		srcVertexShaderBlob->GetBufferPointer(),
		srcVertexShaderBlob->GetBufferSize()
	};
	//shaderBlobをセット(Pixel)
	graghicsPipeLineStatedesc.PS =
	{
		srcPixelShaderBlob->GetBufferPointer(),
		srcPixelShaderBlob->GetBufferSize()
	};


	//DescriptorRangeの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange[1];
	descriptorRange[0] = GetShaderViewDescriptorRange();

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	//バイリニアフィルター
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0～1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのmipMapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixcelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	//RootParameter
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	//CBVを使う(b0の0)
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixcelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号0とバインド
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertexShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//レジスタ番号0とバインド
	rootParameters[1].Descriptor.ShaderRegister = 0;

	//Descriptortableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pixcelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//tableで利用する
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pixcelShaderを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;

	//CBVを使う
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pixcelShaderを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[4].Descriptor.ShaderRegister = 2;


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);
	//ルートパラメータ配列へのポインタ
	rootSignatureDesc.pParameters = rootParameters;
	//配列の長さ
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	//pixcelShaderで読むConstantBufferのBind情報を追加する
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリをもとにrootSignatureを作成
	hr = device_->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&ret_pipelineSet->rootSignature));
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create rootSignature\n");

	//RootSignature
	graghicsPipeLineStatedesc.pRootSignature = ret_pipelineSet->rootSignature.Get();
	//InputLayOut
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	inputElementDescs[0].SemanticName = "SV_POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
	graghicsPipeLineStatedesc.InputLayout = inputLayoutDesc;


	D3D12_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode_) {
	case kBlendModeNormal:

		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


		break;
	case kBlendModeAdd:
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		break;
	}

	graghicsPipeLineStatedesc.BlendState = blendDesc;
	//ラスタライザーステート
	D3D12_RASTERIZER_DESC rasterizerDesc = GetRasterizerDesc(cullMode_);
	graghicsPipeLineStatedesc.RasterizerState = rasterizerDesc;
	//書き込むRTVの情報
	graghicsPipeLineStatedesc.NumRenderTargets = 1;
	graghicsPipeLineStatedesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ（形状）のタイプ三角形
	graghicsPipeLineStatedesc.PrimitiveTopologyType = topologyType_;//D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
	//どのようにして色を打ち込むかの設定
	graghicsPipeLineStatedesc.SampleDesc.Count = 1;
	graghicsPipeLineStatedesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	//depthの機能を有効化する
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	//書き込みする
	if (blendMode_ == kBlendModeNormal)
	{
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}

	else if (blendMode_ == kBlendModeAdd)
	{
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	//比較関数はLessEqual。つかり近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graghicsPipeLineStatedesc.DepthStencilState = depthStencilDesc;
	graghicsPipeLineStatedesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = device_->CreateGraphicsPipelineState(&graghicsPipeLineStatedesc,
		IID_PPV_ARGS(&ret_pipelineSet->pipelineStateObject));
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create graghicsPipelineState\n");

	return ret_pipelineSet;
}

void DxCommon::CreateAllGraphicsPipelineSets(ID3D12Device* device_)
{
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_,D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, ShaderSet::Type::kDefaulAndKirei);

	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, ShaderSet::Type::kDefaulAndKirei);

	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, ShaderSet::Type::kDefaulAndKirei);


	pipelineSetLines[kBlendModeNormal][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kFront, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kFront, ShaderSet::Type::kDefaultAndTest);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kBack, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kBack, ShaderSet::Type::kDefaultAndTest);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kNone, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kNone, ShaderSet::Type::kDefaultAndTest);

	//--------------------------------------------------------------------------------------------------------------------------------
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, ShaderSet::Type::kDefaulAndKirei);

	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, ShaderSet::Type::kDefaulAndKirei);

	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, ShaderSet::Type::kDefaultAndTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSet::Type::kDefaulAndKirei] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, ShaderSet::Type::kDefaulAndKirei);


	pipelineSetLines[kBlendModeAdd][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kFront, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kFront][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kFront, ShaderSet::Type::kDefaultAndTest);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kBack, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kBack][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kBack, ShaderSet::Type::kDefaultAndTest);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndDefault] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kNone, ShaderSet::Type::kDefaultAndDefault);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kNone][ShaderSet::Type::kDefaultAndTest] =
		CreateGraphicsPipelineSet(device_, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kNone, ShaderSet::Type::kDefaultAndTest);

}


void DxCommon::BeginFrame()
{
	//time += deltaTime;
	//commonVariablesData->time = time;

	////キーボード情報の取得
	//inputs.keyboard->Acquire();
	//inputs.keyboard->GetDeviceState(sizeof(*key) * 256, key);
	////マウスの情報の取得
	//inputs.mouse->Acquire();
	//inputs.mouse->GetDeviceState(sizeof(*mouseState_), mouseState_);

	//Imguiにここからフレームが始まる旨を告げる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//描画先のRTV、DSVを設定する
	depthStencilSetUp.dsvHandle = descriptorHeapSet.dH_dsv->GetCPUDescriptorHandleForHeapStart();

	D3D12_RESOURCE_BARRIER barrier{};

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChainControll.swapChain->GetCurrentBackBufferIndex();
	//バリアの設定
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = swapChainControll.swapChain_resourcesAndHandles[backBufferIndex].resource.Get();
	//バリアステートを遷移
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//TransitionBarrierを張る
	commandControll.commandList->ResourceBarrier(1, &barrier);

	commandControll.commandList->OMSetRenderTargets(1,
		&swapChainControll.swapChain_resourcesAndHandles[backBufferIndex].rtvHandle, false, &depthStencilSetUp.dsvHandle);
	//指定した深度で画面クリアする
	commandControll.commandList->ClearDepthStencilView(depthStencilSetUp.dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//指定した色で画面全体をクリアする
	commandControll.commandList->ClearRenderTargetView(
		swapChainControll.swapChain_resourcesAndHandles[backBufferIndex].rtvHandle, windowColor, 0, nullptr);
	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeapSet.dH_srv.Get() };
	commandControll.commandList->SetDescriptorHeaps(1, descriptorHeaps);
	//DXの行列の設定
	//D3D12_VIEWPORT viewPort{};
	//D3D12_RECT scissorRect{};
	//SetDXMatrix(viewPort, scissorRect);
	commandControll.commandList->RSSetViewports(1,
		&swapChainControll.swapChain_resourcesAndHandles[0].viewport);

	commandControll.commandList->RSSetScissorRects(1, 
		&swapChainControll.swapChain_resourcesAndHandles[0].scissorRect);
}
void DxCommon::EndFrame()
{
	//DrawIndexのリセット
	//DrawIndexReset();

	ImGui::Render();
	//実際のcommandListのImguiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandControll.commandList.Get());

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChainControll.swapChain->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER barrier{};
	//バリアの設定
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = 
		swapChainControll.swapChain_resourcesAndHandles[backBufferIndex].resource.Get();

	//画面に書く処理が終わり、画面に映すので状態を遷移
	//RenderTarget->Prsent
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//TransitionBarrierを張る
	commandControll.commandList->ResourceBarrier(1, &barrier);

	//コマンドリストの内容を確定させる
	HRESULT hr = commandControll.commandList->Close();
	assert(SUCCEEDED(hr));

	//GPUにコマンドリストの実行を行わさせる
	ID3D12CommandList* commandLists[] = { commandControll.commandList.Get() };
	commandControll.commandQueue->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知する
	swapChainControll.swapChain->Present(1, 0);

	//イベントを待つ
	fenceControll.WaitFenceEvent(commandControll.commandQueue.Get());
	//次のフレーム用のコマンドリストを準備
	commandControll.PrepareForNextCommandList();

	//fpsController.TimeAdjust();
	//deltaTime = 1.0f / ImGui::GetIO().Framerate;

}

void DxCommon::SetBarrierState(D3D12_RESOURCE_BARRIER& dst_barrier_, D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_)
{
	dst_barrier_.Transition.StateBefore = before_;
	dst_barrier_.Transition.StateAfter = after_;
}
