struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITOIN0;

};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};

struct CommonVariables
{
    float time;
    float3 camera_world;
};

void BinaryAlpha(float4 color_, float min_)
{
    if (color_.a <= min_)
    {
        discard;
    }
}

float GetSpecularPowWithPRM(float3 world_cPos_, float3 world_pos_, float3 normal_, float3 lightDir_, float shininess_)
{
    
    float3 toEye = normalize(world_cPos_ - world_pos_);
    float3 dotlightnormal = dot(lightDir_, normal_);
    float3 reflect = normalize(-lightDir_ + 2 * dotlightnormal * normal_);
    return pow(saturate(dot(reflect, toEye)), shininess_);
    
    //float3 toEye = normalize(world_cPos_ - world_pos_);
    //float3 reflectLight = reflect(-lightDir_, normal_);
    //float RdotE = dot(reflectLight, toEye);
    //return pow(saturate(dot(reflectLight, toEye)), shinnes_);
}

float GetSpecularPowWithB_PRM(float3 world_cPos_, float3 world_pos_, float3 normal_, float3 lightDir_, float shininess_)
{
    float3 toEye = normalize(world_cPos_ - world_pos_);
    float3 halfVec = normalize(-lightDir_ + toEye);
    float NdotH = dot(normal_, halfVec);
    return pow(saturate(NdotH), shininess_);
}