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


float3 Palette(float t)
{
    float3 a = float3(0.5, 0.5, 0.5);
    float3 b = float3(0.5, 0.5, 0.5);
    float3 c = float3(1, 1, 1);
    float3 d = float3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}


PixcelShaderOutput main(VertexShaderOutput input)
{
    if (input.texcoord.x == 0 && input.texcoord.y == 0)
    {
        discard;
    }

    PixcelShaderOutput output;
    //uv座標を -1.0～1.0
    float2 uv = (input.texcoord - 0.5f) * 2.0f;
    //初期値を記憶
    float2 uv0 = uv;
    //最終的に算出する色
    float3 finalCol = float3(0, 0, 0);
    
    for (float i = 0; i < 2; ++i)
    {
        uv *= 1.5f;
        //画面分割
        uv = uv - floor(uv) - 0.5f;
        //円を作る
        float d = length(uv) * exp(-length(uv0));
        //パレット関数に入れて色を遷移させる
        float3 col = Palette(length(uv0) + gCommonVariables.time * 0.5f);
        //しましまにする
        d = sin(d * 8 + gCommonVariables.time * 0.5f);
        d = abs(d);
        d = pow(0.05 / d, 1.3f);
        finalCol += col * d;
    }
    
    //finalCol/=25.0f;
    
    //透明化
    float mag = length(finalCol);
    
    output.color = float4(finalCol, mag);
 
    
    //Lightingをする場合
    if (gMaterial.enableLighting == 1)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color.rgb = gMaterial.color.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }
    else if (gMaterial.enableLighting == 2)
    {
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.intensity));
        output.color.rgb = gMaterial.color.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }

    output.color.a *= gMaterial.color.a;
    
    BinaryAlpha(output.color, 0.0f);

    return output;

}


