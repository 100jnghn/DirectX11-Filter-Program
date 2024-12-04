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

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

cbuffer LightBufferPhong
{
    float4 diffuseColorPhong;
    float3 lightDirectionPhong;
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
    
    float3 lightDir;
    float lightIntensity;
    
    // ----- Phong 조명값 ----- //
    float3 lightDirPhong;
    float lightIntensityPhong;
    // ----------------------- //
    
    float4 color;
    
    float4 ambientColor;

	
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
    lightDir = -lightDirection;
    
	// Calculate the amount of light on this pixel based on the normal map value.
    lightIntensity = saturate(dot(bumpNormal, lightDir));
    
    
    
    // ----- Phong 조명 설정 ----- //
    lightDirPhong = -lightDirectionPhong;
    lightDirPhong = normalize(lightDirPhong);   // 방향 벡터 정규화
    
    lightIntensityPhong = saturate(dot(bumpNormal, lightDirPhong)); // Diffuse  // Normal & Dir 벡터 내적 후 0~1값으로 변환
    //lightIntensityPhong = saturate(dot(input.normal, lightDirPhong)); 
    
    float3 viewingVec = normalize(-input.position.xyz); // View 벡터 == input의 반대 방향
    
    //float3 reflectionVec = reflect(lightDirPhong, input.normal);        // Reflect 벡터   // input.normal 기준
    float3 reflectionVec = reflect(lightDirPhong, bumpNormal);          // Reflect 벡터   // bump normal 기준
    
    float sLightIntensityPhong = saturate(pow(dot(reflectionVec, viewingVec), 16)); // Specular // Reflect & View 벡터 내적 후 0~1값 변환 후 지수 연산
    // -------------------------- //
    
    
    
    
    
    color = saturate(diffuseColor * lightIntensity + (diffuseColorPhong * lightIntensityPhong + diffuseColorPhong * sLightIntensityPhong));
    //color = saturate(diffuseColor * lightIntensity + diffuseColorPhong * sLightIntensityPhong);

	// Combine the final light color with the texture color.
    color = color * textureColor * 1.3;     // 1.3은 임시 값!

    //return textureColor;
    return color;
}
