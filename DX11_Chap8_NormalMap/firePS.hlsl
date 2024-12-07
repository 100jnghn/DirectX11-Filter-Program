////////////////////////////////////////////////////////////////////////////////
// Filename: fire.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
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
	float4 textureColor;


	 // Translate the position where we sample the pixel from.
    input.tex.y -= textureTranslation;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}
