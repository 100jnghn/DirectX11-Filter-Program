////////////////////////////////////////////////////////////////////////////////
// Filename: glass.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
SamplerState SampleType : register(s0);
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D refractionTexture : register(t2);

cbuffer GlassBuffer
{
    float refractionScale;
    float3 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 refractionPosition : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 GlassPixelShader(PixelInputType input) : SV_TARGET
{
    float2 refractTexCoord;
    float4 normalMap;
    float3 normal;
    float4 refractionColor;
    float4 textureColor;
    float4 color;

    // refraction texture ��ǥ ���
    refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
    refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

    // Normal map ���ø�
    normalMap = normalTexture.Sample(SampleType, input.tex);

    // ���� ������: (0,1) to (-1,+1).
    normal = (normalMap.xyz * 2.0f) - 1.0f;
    
    refractTexCoord = refractTexCoord + (normal.xy * refractionScale);

    // ����� refraction texture ��ǥ�� ����� refractionTexture ���ø�
    refractionColor = refractionTexture.Sample(SampleType, refractTexCoord);

    // Ice Texture ���ø�
    textureColor = colorTexture.Sample(SampleType, input.tex);

    // Ice, Refraction�� ������ ���� ����
    color = lerp(refractionColor, textureColor, 0.5f);

    return color;
}