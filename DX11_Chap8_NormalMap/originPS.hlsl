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

cbuffer ShiftColorBuffer
{
    float4 shiftColor;
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
float4 OriginPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    
    float3 lightDir;
    float lightIntensity;
    
    int ex;
    float3 viewingVec;
    float3 reflectVec;
    float sLightIntensity;
    
    float4 color;
    float4 ambientColor;
    
    // ----- texture sampling ----- //
    textureColor = shaderTexture1.Sample(SampleType, input.tex);
    bumpMap = shaderTexture2.Sample(SampleType, input.tex);
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    
    // ----- normal 구하기 ----- //
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    bumpNormal = normalize(bumpNormal);
    
    
    
    // ----- 조명의 방향 ----- //
    lightDir = -lightDirection;
    lightDir = normalize(lightDir); // 방향 벡터 정규화
    
    // ----- Norm & Dir 벡터 내적 후 0~1값으로 치환 ----- //
    lightIntensity = saturate(dot(bumpNormal, lightDir));   // diffuse
    
    // ----- View 벡터 ----- //
    viewingVec = normalize(-input.position.xyz);    // View 벡터 == input의 반대 방향
    
    // ----- Reflect 벡터 ----- //
    reflectVec = reflect(lightDir, bumpNormal);
    
    // ----- Specular ----- //
    ex = 64;
    sLightIntensity = saturate(pow(dot(reflectVec, viewingVec), ex));   // ReflecVec & ViewVec 내적 후 0~1값 치환 후 지수 연산
    
    
    
    // ----- Phong Shading 계산 식 ----- //
    color = saturate((diffuseColor * lightIntensity) + (diffuseColor * sLightIntensity));
    
    
    
    

	// Combine the final light color with the texture color.
    color = saturate(color * textureColor * 1.3); // 1.3은 임시 값!
    
    // ---- filter에 따른 색상 연산 추가 ----- //
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    

    
    return color;
}
