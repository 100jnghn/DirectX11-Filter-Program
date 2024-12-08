////////////////////////////////////////////////////////////////////////////////
// Filename: fire.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture1 : register(t0);
Texture2D shaderTexture2 : register(t1);
Texture2D alphaTexture : register(t2);
SamplerState SampleType : register(s0);

cbuffer TranslationBuffer
{
    float textureTranslation;
    float bright;
    
    float2 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 FirePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor1;
    float4 textureColor2;
    float4 alphaValue;
    float4 color;
    
    input.tex.y -= textureTranslation;
    
    alphaValue = alphaTexture.Sample(SampleType, input.tex - textureTranslation * 0.1f);

    
    textureColor1 = shaderTexture1.Sample(SampleType, input.tex + textureTranslation * 0.2f);
    textureColor2 = shaderTexture2.Sample(SampleType, input.tex - textureTranslation * 0.2f);
    
    // bright 값이 커지면 어두워짐   // bright 값이 작아지면 밝아짐
    color = (textureColor1 * textureColor2) / bright;

    float points = alphaValue.x + alphaValue.y + alphaValue.z;
    
    if (points <= 0.2)          color.a = 0.7f;
    else if (points <= 0.3f)    color.a = 0.8f;
    else if (points <= 0.5f)    color.a = 0.9f;
    else                        color.a = 1.0f;

    return color;
}
