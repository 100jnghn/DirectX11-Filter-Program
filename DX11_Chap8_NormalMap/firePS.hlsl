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
    
    alphaValue = alphaTexture.Sample(SampleType, input.tex);

	// Translate the position where we sample the pixel from.
    
    textureColor1 = shaderTexture1.Sample(SampleType, input.tex);
    textureColor2 = shaderTexture2.Sample(SampleType, input.tex);
    
    
    color = (textureColor1 * textureColor2) / 2.0f;

    float points = alphaValue.x + alphaValue.y + alphaValue.z;
    if (points <= 0.2)          color.a = 0.7f;
    else if (points <= 0.4f)    color.a = 0.8f;
    else if (points <= 0.6f)    color.a = 0.9f;
    else                        color.a = 1.0f;

    return color;
}
