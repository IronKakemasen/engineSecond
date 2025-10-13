#pragma once
#include <wrl.h>
#include <dxcapi.h>
#include <unordered_map>
#include <string>

struct PixelShaderData
{
	enum Type
	{
		kDefault,
		kKirei,
		kTest,

		kCount

	};

	std::unordered_map<Type, std::pair<std::string, Microsoft::WRL::ComPtr<IDxcBlob>>> data;

	PixelShaderData();
};