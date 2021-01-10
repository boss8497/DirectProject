
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer : register(b3)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

cbuffer TextureBuffer : register(b4)
{
	float4 textureBufferColor;
};


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXTCOORD0;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	//�ؽ�ó ��ǥ ��ġ���� ���÷��� ����Ͽ� �ؽ�ó���� �ȼ� ������ ���ø�
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	//��� �ȼ��� �⺻ ��� ������ �ֺ� ���� ������ ����
	color = ambientColor;


	/*�׳� ��ǥ�� ������ �ϴ÷� ��� ���Ͷ� ���� ��Ű�� ����*/
	//����� ���� �� ������ ����
	lightDir = -lightDirection;

	/*saturate()��� �Լ��� 0 ������ ���� 0����, 1 �̻��� ���� 1�� �ٲ��ݴϴ�.*/
	//�ȼ��� ���� ���� ���
	lightIntensity = saturate(dot(input.normal, lightDir));


	if (lightIntensity > 0.0f)
	{
		//������ �Ǹ� Ȯ�� ���� �� ������ �翡 ���� ���� Ȯ�� ���� ����
		color += (diffuseColor * lightIntensity);
	}

	//���� ���� ������ ä��
	color = saturate(color);

	//�ؽ�ó �ȼ��� ���� Ȯ�� ���� ���Ͽ� ���� �ȼ� ���� ����� ����
	color = color * textureColor * textureBufferColor;


	return color;
}