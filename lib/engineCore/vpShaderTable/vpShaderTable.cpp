#include "VPShaderTable.h"
#include "../allShaderData/vertexShaderData.h"
#include "../allShaderData/pixelShaderData.h"




void VPShaderTable::Initilaize()
{

}

//シェーダセットのテーブルを設定
void VPShaderTable::Set(ShaderSet::Type setType_,
	IDxcBlob* src_vertexShaderBlob_, IDxcBlob* src_pixelShaderBlob_)
{
	table[setType_].verShaderBlob = src_vertexShaderBlob_;
	table[setType_].pixShaderBlob = src_pixelShaderBlob_;
}

ShaderSet VPShaderTable::Get(ShaderSet::Type type_)
{
	return table[type_];
}
