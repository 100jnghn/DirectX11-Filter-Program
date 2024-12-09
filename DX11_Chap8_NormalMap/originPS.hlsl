////////////////////////////////////////////////////////////////////////////////
// Filename: normalmap.ps
////////////////////////////////////////////////////////////////////////////////


// ----- PS hlsl�� �Ӽ����� Shader Model�� 6.0���� �����Ͽ� input.position�� ���� ����� ----- //


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
    
    // ----- normal ���ϱ� ----- //
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    bumpNormal = normalize(bumpNormal);
    
    
    
    // ----- ������ ���� ----- //
    lightDir = -lightDirection;
    lightDir = normalize(lightDir); // ���� ���� ����ȭ
    
    // ----- Norm & Dir ���� ���� �� 0~1������ ġȯ ----- //
    lightIntensity = saturate(dot(bumpNormal, lightDir));   // diffuse
    
    // ----- View ���� ----- //
    viewingVec = normalize(-input.position.xyz);    // View ���� == input�� �ݴ� ����
    
    // ----- Reflect ���� ----- //
    reflectVec = reflect(lightDir, bumpNormal);
    
    // ----- Specular ----- //
    ex = 64;
    sLightIntensity = saturate(pow(dot(reflectVec, viewingVec), ex));   // ReflecVec & ViewVec ���� �� 0~1�� ġȯ �� ���� ����
    
    
    
    // ----- Phong Shading ��� �� ----- //
    color = saturate((diffuseColor * lightIntensity) + (diffuseColor * sLightIntensity));
    
    
    
    

	// Combine the final light color with the texture color.
    color = saturate(color * textureColor * 1.3); // 1.3�� �ӽ� ��!
    
    // ---- filter�� ���� ���� ���� �߰� ----- //
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    

    
    return color;
}
