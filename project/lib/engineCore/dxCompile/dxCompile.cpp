#include "dxCompile.h"
#include <assert.h>
#include "../dxCommon/dxCommon.h"
#include <format>

#pragma comment(lib,"dxcompiler.lib")


Microsoft::WRL::ComPtr<IDxcBlob> DxCompile::CompileShader(
	std::string const& fileName_,
	const wchar_t* profile_,
	IDxcUtils* dxcUtils_,
	IDxcCompiler3* dxcCompiler_,
	IDxcIncludeHandler* includeHandler_)
{
	std::string const filePath = "./resource/preset/shader/";
	std::string const fileType  = ".hlsl";
	std::string compositePath = filePath + fileName_ + fileType;

	std::wstring convert_filePath = ConvertString(compositePath);


	//[ 1.hlslファイルを読み込む ]
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", convert_filePath, profile_)));
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(convert_filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete load shader\n");

	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	//UTF-8のもじであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//[ 2.コンパイルする ]
	LPCWSTR arguments[] =
	{
		//コンパイル対象のhlslファイル名
		convert_filePath.c_str(),
		//エントリーポイントの指定。基本的にmain以外にしない
		L"-E",L"main",
		//shaderProfileの設定
		L"-T",profile_,
		//デバッグ用の情報を埋め込む
		L"-Zi",L"-Qembed_debug",
		//最適化を外す
		L"-Od",
		//メモリレイアウトは行優先
		L"-Zpr"
	};

	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		//読み込んだファイル
		&shaderSourceBuffer,
		//コンパイルオプション
		arguments,
		//コンパイルオプションの数
		_countof(arguments),
		//includeが含まれたもろもろ
		includeHandler_,
		IID_PPV_ARGS(&shaderResult)
	);
	//dxcが起動できないなどの致命的な状況
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, "Complete compile\n");

	//[ 3.エラー、警告確認 ]
	//警告エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		assert(false);
	}

	//[ 4.コンパイル結果を受け取る ]
	//コンパイル結果から実行用のバイナリ部分を受け取る
	Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	Log(DxCommon::debugLog, ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", convert_filePath, profile_)));

	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリをリターン
	return shaderBlob;
}


void DxCompile::SetDXcCompiler()
{
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}

void DxCompile::Initialize()
{
	SetDXcCompiler();
}
