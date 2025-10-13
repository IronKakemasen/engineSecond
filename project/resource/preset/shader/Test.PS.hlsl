#include "dousite.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

//コンスタントバッファの定義
//b = constantBuffer,0 = shader上でのresourceナンバー
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<CommonVariables> gCommonVariables : register(b2);


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};


PixcelShaderOutput main(VertexShaderOutput input) 
{
    PixcelShaderOutput output;

       
    float radius = 0.05f + 0.05f * cos(gCommonVariables.time) * sin(gCommonVariables.time * 0.05f);
    float2 segABStart = { 0.4f + 0.2f * sin(gCommonVariables.time * 0.2f), 0.75f };
    float2 segABEnd = { 0.5f + 0.2f * sin(gCommonVariables.time * 0.2f), 0.25f };

    float2 pointC = { clamp(sin(gCommonVariables.time * 0.2f), 0.4f, 0.75f), clamp(sin(1.57f + gCommonVariables.time * 0.5f), 0.4f, 0.75f) };
    
    float2 segAB = segABEnd - segABStart;
    
    float2 segAP = input.texcoord - segABStart;
    float t = dot(segAB, segAP) / dot(segAB, segAB);
    t = clamp(t, 0, 1.0f);
    float2 nearPoint = segAB * t;
    float diff1 = length(segAP - nearPoint) - radius;
    float diff2 = length(input.texcoord - pointC) - radius;
    float d = min(diff1, diff2);
       
    if (d >= -0.01f)
    {
        output.color = float4(0.3f, 0.9f, 0.6f, 1.0f);
    }
    else
    {
        output.color = float4(0.0f, 1.0f, 2.0f, 1.0f);
    }
    
    //output.color = float4(0.0f, 1.0f, 2.0f, 1.0f);

    
    float tmp = 100.0f;
    output.color.rgb *= exp(-tmp * abs(d));
    output.color.rgb *= smoothstep(0.3f, 0.6f, abs(cos(-d * 70.0f)));
    
    
        //Lightingをする場合
    if (gMaterial.enableLighting == 1)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color.rgb = gMaterial.color.rgb  * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }
    else if (gMaterial.enableLighting == 2)
    {
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.intensity));
        output.color.rgb = gMaterial.color.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }


    //float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    //float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
    //output.color.rgb *= gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;

    BinaryAlpha(output.color, 0.0f);
    
    return output;

}


