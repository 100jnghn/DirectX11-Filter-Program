////////////////////////////////////////////////////////////////////////////////
// Filename: normalmap.ps
////////////////////////////////////////////////////////////////////////////////


// ----- PS hlsl의 속성에서 Shader Model을 6.0으로 변경하여 input.position에 접근 허용함 ----- //


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture1 : register(t0);
Texture2D shaderTexture2 : register(t1);
SamplerState SampleType : register(s0);

cbuffer LightBuffer1
{
    float4 diffuseColor1;
    float3 lightDirection1;
    float padding;
};

cbuffer LightBuffer2
{
    float4 diffuseColor2;
    float3 lightDirection2;
    float paddingPhong;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 NormalMapPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    
    // ----- 조명1 ----- //
    float3 lightDir1;
    float lightIntensity1;
    
    // ----- 조명2 ----- //
    float3 lightDir2;
    float lightIntensity2;
    
    // 최종 결과를 저장할 변수
    float4 color;

    
	
	// Sample the pixel color from the color texture at this location.
    textureColor = shaderTexture1.Sample(SampleType, input.tex);

	// Sample the pixel from the normal map.
    bumpMap = shaderTexture2.Sample(SampleType, input.tex);
    
	// Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
	// Calculate the normal from the data in the normal map.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    
	// Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);
    
	// Invert the light direction for calculations.
    lightDir1 = -lightDirection1;
    
	// Calculate the amount of light on this pixel based on the normal map value.
    lightIntensity1 = saturate(dot(bumpNormal, lightDir1));
    
    lightDir2 = -lightDirection2;
    
	// Calculate the amount of light on this pixel based on the normal map value.
    lightIntensity2 = saturate(dot(bumpNormal, lightDir2));
    
    
 
	// Combine the final light color with the texture color.
    float4 color1 = saturate(diffuseColor1 * lightIntensity1);
    float4 color2 = saturate(diffuseColor2 * lightIntensity2);
    color = saturate(color1 + color2);
    color = color * textureColor * 1.3;     // 1.3은 임의의 값


    
    return color;
}
