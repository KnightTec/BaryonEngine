#include "Baryon.hlsl"

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

void applyFog(in float2 texCoords, inout float3 color)
{
    float3 worldPos = getWorldSpacePosition(texCoords);
    float3 viewDir = worldPos - cameraPosition;
    float distance = length(viewDir);
    viewDir = normalize(viewDir);
    float falloff = 0.3;

    float fogFactorDepth = 1 - exp(-distance * falloff);
    
    float fogFactorHeight = exp(-cameraPosition.y * falloff) * (1.0 - exp(-distance * viewDir.y * falloff)) / (viewDir.y * falloff);
    fogFactorHeight = saturate(fogFactorHeight);

    float fogFactor = fogFactorDepth * fogFactorHeight;

    float3 fogColor = float3(0.8, 0.8, 0.8);

    float3 lightDir = normalize(float3(8, 3, -4));
    float sunFactor = saturate(dot(lightDir, viewDir));

    fogColor = lerp(float3(0.55, 0.6, 0.75), float3(1, 0.8, 0.75), sunFactor);

    color = fogFactor * fogColor + (1 - fogFactor) * color;
}

float4 main(in VSOutput input) : SV_Target0
{
    float3 nor = gBufferTexture1.Sample(texSampler, input.tex).xyz;    
    
    float3 lightDir = normalize(float3(8, 3, -4));

    float3 surfaceColor = gBufferTexture0.Sample(texSampler, input.tex).rgb;

    float3 worldPos = getWorldSpacePosition(input.tex);
    float3 viewDir = normalize(cameraPosition.xyz - worldPos);
    float3 halfVec = normalize(lightDir + viewDir);
    float specularExponent = 50;
    float specularIntensity = 1;
    specularIntensity *= pow(saturate(dot(nor, halfVec)), specularExponent);
    float3 specularColor = float3(1, 0.8, 0.75);
    float diffuseIntensity = saturate(dot(lightDir, nor));

    // prevent specular reflection in areas not facing the light
    specularIntensity *= ceil(diffuseIntensity);

    float3 diffuseLight = float3(1, 0.8, 0.75) * diffuseIntensity;
    float3 ambientLight = 0.2f * float3(0.55, 0.6, 0.75);
    float3 color = (diffuseLight + ambientLight) * surfaceColor + specularIntensity * specularColor;
  
    if (length(nor) == 0)
    {
        color = float3(0.00, 0.00, 0.00);
    }

    applyFog(input.tex, color);

    return float4(color, 1);
}