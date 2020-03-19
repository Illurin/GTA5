#include "LightingUtil.hlsl"

struct PixelIn {
	float4 position : SV_POSITION;
	float3 posW;
	float2 texCoord;
	float3 normal;
	float4 shadowPos;
};

[vk::binding(2, 0)]
cbuffer MaterialConstants {
	float4x4 matTransform;
	float4 diffuseAlbedo;
	float3 fresnelR0;
	float roughness;
};

[vk::binding(2, 1)]
SamplerState samplerState;

[vk::binding(1, 0)]
Texture2D tex;

float4 main(PixelIn input) : SV_TARGET
{
	//�����õ�����ɫ
	float4 sampleColor = tex.Sample(samplerState, input.texCoord);

	//��������յĻ�������
	input.normal = normalize(input.normal);
	float3 toEye = normalize(eyePos.xyz - input.posW);

	//�����������ɫ
	float4 diffuse = sampleColor * diffuseAlbedo;

	//���ϲ���
	const float shiniess = 1.0f - roughness;
	Material mat = { diffuseAlbedo, fresnelR0, shiniess };

	//�������Ӱ����
	float shadowFactor = CalcShadowFactor(input.shadowPos);

	//��������յ�ֵ
	float3 lightingResult = shadowFactor * ComputePointLight(lights[0], mat, input.posW, input.normal, toEye);

	//��������յ���ɫ���
	float4 litColor = diffuse * (ambientLight + float4(lightingResult, 1.0f));
	litColor.a = sampleColor.a;

	return litColor;
}