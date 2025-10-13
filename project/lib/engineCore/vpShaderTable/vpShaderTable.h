#pragma once

#include <string>
#include <unordered_map>

struct IDxcBlob;



struct ShaderSet
{
	enum Type
	{
		kDefaultAndDefault,
		kDefaultAndTest,
		kDefaulAndKirei,
		kCount
	};

	IDxcBlob* verShaderBlob = nullptr;
	IDxcBlob* pixShaderBlob = nullptr;


};

struct VPShaderTable
{
	std::unordered_map<ShaderSet::Type, ShaderSet> table;

	//シェーダセットのテーブルを設定
	void Set(ShaderSet::Type setType_,
		IDxcBlob* src_vertexShaderBlob_, IDxcBlob* src_pixelShaderBlob_);

	ShaderSet Get(ShaderSet::Type type_);


	void Initilaize();

};
