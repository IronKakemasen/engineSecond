#include "dousite.hlsli"


struct TransformationMatrix
{
	float4x4 WVP;
    float4x4 World;

};

struct VertexShaderInput
{
	float3 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix: register(b0);
ConstantBuffer<CommonVariables> gCommonVariables : register(b2);


VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.texcoord = input.texcoord;
    float4 tmp = float4(input.position.x, input.position.y, input.position.z,1.0f);
    
    output.position = mul(tmp, gTransformationMatrix.WVP);
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.World));
    output.worldPosition = mul(tmp, gTransformationMatrix.World).xyz;
    
	return output;
}



