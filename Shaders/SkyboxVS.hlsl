#include "Common.hlsl"

struct VertexOut {
	float4 position : SV_POSITION;
	float3 posL;
};

struct VertexIn {
	float3 posH;
	float2 texCoord;
	float3 normal;
};

[vk::binding(0, 0)]
cbuffer ObjectConstants {
	float4x4 worldMatrix;
};

VertexOut main(VertexIn input)
{
	VertexOut output;

	//�þֲ������λ����Ϊ������ͼ�Ĳ�������
	output.posL = input.posH;

	//�Ѷ���任����βü��ռ�
	float4 posW = mul(worldMatrix, float4(input.posH, 1.0f));

	//�������������Ϊ����������
	posW.xyz += eyePos;

	float4x4 viewProjMatrix = mul(projMatrix, viewMatrix);
	posW = mul(viewProjMatrix, posW);

	//ʹz = w ����������λ��Զƽ��
	output.position = posW.xyww;

	return output;
}