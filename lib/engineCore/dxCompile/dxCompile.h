#pragma once
#include <wrl.h>
#include <dxcapi.h>
#include <unordered_map>
#include <string>

struct DxCompile
{
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	//Dxのコンパイラーを設定する
	void SetDXcCompiler();
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		std::string const& fileName_,
		const wchar_t* profile_,
		IDxcUtils* dxcUtils_,
		IDxcCompiler3* dxcCompiler_,
		IDxcIncludeHandler* includeHandler_);

	void Initialize();
};

