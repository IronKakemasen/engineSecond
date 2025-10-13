#include "dousite.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

//コンスタントバッファの定義
//b = constantBuffer,0 = shader上でのresourceナンバー
ConstantBuffer<Material>gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<CommonVariables> gCommonVariables : register(b2);


struct PixcelShaderOutput
{
	float4 color : SV_TARGET0;
};


PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord.x, input.texcoord.y, 1.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float specularPow = GetSpecularPowWithPRM(gCommonVariables.camera_world, input.worldPosition, input.normal,
    gDirectionalLight.direction, 1000.0f);
    
    
 //   float3 toEye = normalize(gCommonVariables.camera_world - input.worldPosition);
 //   // ライトに向かうベクトルと法線の内積
 //   float3 dotlightnormal = dot(gDirectionalLight.direction, input.normal);
	//// 反射光ベクトル
 //   float3 reflect = normalize(-gDirectionalLight.direction + 2 * dotlightnormal * input.normal);
	//// 鏡面反射光
 //   float3 specularPow = pow(saturate(dot(reflect, toEye)), 40.0f);

    //float3 toEye = normalize(world_cPos_ - world_pos_);
    //float3 reflectLight = reflect(lightDir_, normal_);
    //return pow(saturate(dot(reflectLight, toEye)), shinnes_);    //float RdotE = dot(reflectLight, toEye);


    //gMaterial.shininess
    
    //Lightingをする場合
    if (gMaterial.enableLighting == 1)  
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb *
        cos * gDirectionalLight.intensity;

        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1, 1, 1);
        output.color.rgb = diffuse + specular;
        
        //output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else if (gMaterial.enableLighting == 2)
    { 
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb *
        cos * gDirectionalLight.intensity;
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1, 1, 1);

        //output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.rgb = diffuse + specular;

        output.color.a = gMaterial.color.a * textureColor.a;

    }
    else
    {
        output.color = gMaterial.color * textureColor;

    }
    
    BinaryAlpha(output.color, 0.0);

    return output;

}

