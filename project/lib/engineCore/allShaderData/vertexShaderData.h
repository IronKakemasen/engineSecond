#pragma once
#include <wrl.h>
#include <dxcapi.h>
#include <unordered_map>
#include <string>


struct VertexShaderData
{
	
	enum Type
	{
		kDefault,
		kCount,


	};

	std::unordered_map<Type, std::pair<std::string, Microsoft::WRL::ComPtr<IDxcBlob>>> data;

	VertexShaderData();



};